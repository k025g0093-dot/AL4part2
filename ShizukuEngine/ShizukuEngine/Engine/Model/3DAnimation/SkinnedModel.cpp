#include "SkinnedModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment(lib,"assimp-vc145-mt")
#include "AssimpGLMHelpersMath/GlmConvert.h"

using namespace std;

SkinnedModel::SkinnedModel() {}
SkinnedModel::~SkinnedModel() {}

const aiScene* SkinnedModel::Load(const std::string& filepath, Assimp::Importer& importer) {
	const aiScene* scene = importer.ReadFile(filepath,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_ValidateDataStructure |
		aiProcess_ImproveCacheLocality |
		aiProcess_SortByPType |
		aiProcess_LimitBoneWeights |
		aiProcess_ConvertToLeftHanded);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		return nullptr;
	}
	return scene;

}

bool SkinnedModel::LoadSkinned(
	ID3D12Device* device,
	const std::string& filepath
) {
	//ファイルが合っていない場合はfalseを返す
	Assimp::Importer importer;
	const aiScene* scene = Load(filepath, importer);
	if (!scene || !scene->mRootNode)return false;
	//スキン用の構造体を使用

	ProcessNode(scene->mRootNode, scene);
	//ProcessMesh(*scene->mMeshes, scene);
	CreateBuffers(device);
	if (scene->mNumMeshes > 0) {
		const unsigned int materialIndex = scene->mMeshes[0]->mMaterialIndex;
		if (materialIndex < scene->mNumMaterials) {
			TexPath(scene->mMaterials[materialIndex], filepath);
		}
	}
	mPMaterialResource = CreateBufferResource(device, Align256(sizeof(Material)));
	UpdateMaterial();

	//---------- 確認用：読み込んだ中身を出力ウィンドウに出す ----------
	{
		glm::vec3 minPos(FLT_MAX);
		glm::vec3 maxPos(-FLT_MAX);
		for (const Vertex& v : mVertices) {
			minPos = (glm::min)(minPos, v.position);
			maxPos = (glm::max)(maxPos, v.position);
		}

		uint32_t maxIndex = 0;
		for (uint32_t index : mIndices) {
			maxIndex = (std::max)(maxIndex, index);
		}

		std::string log;
		log += "[SkinnedModel] " + filepath + "\n";
		log += "  meshes   : " + std::to_string(scene->mNumMeshes) + "\n";
		log += "  vertices : " + std::to_string(mVertices.size()) + "\n";
		log += "  indices  : " + std::to_string(mIndices.size())
			+ " (max index " + std::to_string(maxIndex) + ")\n";
		log += "  bones    : " + std::to_string(mBoneCounter) + "\n";
		log += "  min      : " + std::to_string(minPos.x) + ", " + std::to_string(minPos.y) + ", " + std::to_string(minPos.z) + "\n";
		log += "  max      : " + std::to_string(maxPos.x) + ", " + std::to_string(maxPos.y) + ", " + std::to_string(maxPos.z) + "\n";
		OutputDebugStringA(log.c_str());
	}
	//------------------------------------------------------------------

	return true;
}


void SkinnedModel::Draw(
	ID3D12GraphicsCommandList* cmdList,
	UINT instanceCount, 
	UINT startInstanceLocation) {

	cmdList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	cmdList->IASetIndexBuffer(&mIndexBufferView);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->SetGraphicsRootConstantBufferView(0, mPMaterialResource->GetGPUVirtualAddress());
	cmdList->DrawIndexedInstanced(mIndexCount, instanceCount, 0, 0, startInstanceLocation);

}

void SkinnedModel::SetVertexBoneDataToDefault(Vertex& vertex) {
	for (int i = 0; i < kMaxBoneIngluence; i++) {
		vertex.mBoneIds[i] = -1;
		vertex.mWeights[i] = 0.0f;
	}
}

void SkinnedModel::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		vertex.position = GlmConvert::GetGLMVec(mesh->mVertices[i]);
		vertex.normal = GlmConvert::GetGLMVec(mesh->mNormals[i]);

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		mVertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			mIndices.push_back(face.mIndices[j]);
	}

	ExtractBoneWeightForVertices(mVertices, mesh, scene);

}


void SkinnedModel::ProcessNode(aiNode* node, const aiScene* scene,
	const glm::mat4& parentTransform)
{
	glm::mat4 nodeTransform = parentTransform * GlmConvert::ConvertMatrixToGLMFormat( node->mTransformation);

	for (UINT i = 0; i < node->mNumMeshes; i++) {
		unsigned int meshIndex = node->mMeshes[i];
		if (meshIndex >= scene->mNumMeshes) continue;
		aiMesh* mesh = scene->mMeshes[meshIndex];
		if (!mesh || !mesh->mVertices) continue;
		UINT baseVertex = (UINT)mVertices.size();
		for (UINT v = 0; v < mesh->mNumVertices; v++) {
			Vertex vert{};
			SetVertexBoneDataToDefault(vert);
			glm::vec4 pos = nodeTransform * glm::vec4(GlmConvert::GetGLMVec(mesh->mVertices[v]),1.0f);
			vert.position = { pos.x, pos.y, pos.z};
			if (mesh->mTextureCoords[0]) {
				vert.texCoords = { mesh->mTextureCoords[0][v].x,  mesh->mTextureCoords[0][v].y };
			}
			if (mesh->mNormals) {
				glm::vec3 nm = glm::normalize(glm::mat3(nodeTransform) * GlmConvert::GetGLMVec(mesh->mNormals[v]));
				vert.normal = { nm.x, nm.y, nm.z };
			}
			if (mesh->mTangents) {
				vert.tangent = { mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z };
			}

			mVertices.push_back(vert);
		}
		for (UINT f = 0; f < mesh->mNumFaces; f++) {
			aiFace& face = mesh->mFaces[f];
			for (UINT idx = 0; idx < face.mNumIndices; idx++)
				mIndices.push_back(baseVertex + face.mIndices[idx]);
		}
		ExtractBoneWeightForVertices(mVertices, mesh, scene);

	}
	for (UINT i = 0; i < node->mNumChildren; i++)
		if (node->mChildren[i])
			ProcessNode(node->mChildren[i], scene, nodeTransform);
}


void SkinnedModel::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < kMaxBoneIngluence; ++i)
	{
		if (vertex.mBoneIds[i] < 0)
		{
			vertex.mWeights[i] = weight;
			vertex.mBoneIds[i] = boneID;
			break;
		}
	}
}

void SkinnedModel::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {

		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (mBoneInfoMap.find(boneName) == mBoneInfoMap.end()) {

			BoneInFo newBoneInfo;
			newBoneInfo.id = mBoneCounter;
			newBoneInfo.offset = GlmConvert::ConvertMatrixToGLMFormat(
				mesh->mBones[boneIndex]->mOffsetMatrix);
			mBoneInfoMap[boneName] = newBoneInfo;
			boneID = mBoneCounter;
			mBoneCounter++;
		}
		else {
			boneID = mBoneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId < vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void SkinnedModel::CreateBuffers(
	ID3D12Device* device
) {
	mVertexCount = (uint32_t)mVertices.size();
	mIndexCount = (uint32_t)mIndices.size();

	//頂点バッファ
	mPVertexResource = CreateBufferResource(device, sizeof(Vertex) * mVertexCount);
	void* pData = nullptr;
	mPVertexResource->Map(0, nullptr, &pData);
	memcpy(pData, mVertices.data(), sizeof(Vertex) * mVertexCount);
	mPVertexResource->Unmap(0, nullptr);

	mVertexBufferView.BufferLocation = mPVertexResource->GetGPUVirtualAddress();
	mVertexBufferView.SizeInBytes = sizeof(Vertex) * (UINT)mVertexCount;
	mVertexBufferView.StrideInBytes = sizeof(Vertex);

	//インデックスバッファー
	mPIndexResource = CreateBufferResource(device, sizeof(uint32_t) * mIndexCount);
	uint32_t* idxData = nullptr;
	mPIndexResource->Map(0, nullptr, reinterpret_cast<void**>(&idxData));
	memcpy(idxData, mIndices.data(), sizeof(uint32_t) * mIndexCount);
	mPIndexResource->Unmap(0, nullptr);

	mIndexBufferView.BufferLocation = mPIndexResource->GetGPUVirtualAddress();
	mIndexBufferView.SizeInBytes = sizeof(uint32_t) * mIndexCount;
	mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
}

void SkinnedModel::TexPath(const aiMaterial* mtl, const std::string& filepath) {
	if (!mtl) {
		return;
	}
	aiString path;
	if (mtl->GetTexture(aiTextureType_DIFFUSE, 0, &path) != AI_SUCCESS) { return; }
	std::filesystem::path texPath = path.C_Str();
	// 相対パスなら、モデルファイルがあるフォルダから見たパスにする
	if (texPath.is_relative()) {
		texPath = std::filesystem::path(filepath).parent_path() / texPath;
	}
	mTexturePath = texPath.lexically_normal().generic_string();
}

//マテリアルの色を変える
void SkinnedModel::SetColor(const Vector4& color) {
	mColor = color;
	UpdateMaterial();
}

//ライティングの有無を変える
void SkinnedModel::SetEnableLighting(bool enableLighting) {
	mEnableLighting = enableLighting;
	UpdateMaterial();
}

//メンバの値をマテリアルのバッファに書き込む
void SkinnedModel::UpdateMaterial() {
	if (!mPMaterialResource) { return; }

	Material* materialData = nullptr;
	mPMaterialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = mColor;
	materialData->enableNormalMap = 0;
	materialData->enableLighting = mEnableLighting ? 1 : 0;
	materialData->uvTransform = MakeIdentity4x4();
	mPMaterialResource->Unmap(0, nullptr);
}
