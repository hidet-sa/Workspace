#include "Common.h"
#include "Dump.h"

//opengl
#include <gl/GLU.h>
#include <gl/GL.h>

//モデル情報をダンプする。
void DumpModel(const aiScene *scene)
{
	Log("Mesh Information");
	Log("HasMeshes:%d", scene->HasMeshes());
	Log("mMeshes:%d", scene->mRootNode->mMeshes);

	//Mesh ------------------------------------------------------------------------------------------
	std::function<void(const struct aiMesh *)> meshFunc = [&](const struct aiMesh *mesh) {
		Log("\t\t\tMesh:%s", mesh->mName.C_Str());
		//頂点情報。
		Log("\t\t\tVertices:%d", mesh->mNumVertices);
		int num_vtx = mesh->mNumVertices;
		for (int i = 0; i < num_vtx; i++) {
			const aiVector3D vtx = mesh->mVertices[i];
			Log("\t\t\t\tVertex(%d):%f,%f,%f", i, vtx.x, vtx.y, vtx.y);
		}
		//法線
		if (mesh->HasNormals()) {
			for (int i = 0; i < num_vtx; i++) {
				const aiVector3D normal = mesh->mNormals[i];
				Log("\t\t\t\tNormal(%d):%f,%f,%f", i, normal.x, normal.y, normal.y);
			}
		}
		//テクスチャ座標
		for (int i = 0; i < ARRAY_SIZE(mesh->mTextureCoords); i++) {
			if (mesh->HasTextureCoords(i)) {
				Log("\t\t\tTextureCoords(%d)", i);
				const aiVector3D* uv = mesh->mTextureCoords[i];
				for (int j = 0; j < num_vtx; j++) {
					Log("\t\t\t\tuv(%d):%f,%f,%f", j, uv[j].x, uv[j].y, uv[j].z);
				}
			}
		}
		// 頂点カラー
		for (int i = 0; i < ARRAY_SIZE(mesh->mColors); i++) {
			if (mesh->HasVertexColors(i)) {
				Log("\t\t\tVertexColors(%d)", i);
				const aiColor4D *color = mesh->mColors[i];
				for (int j = 0; j < num_vtx; j++) {
					Log("\t\t\t\tcolor(%d):%f,%f,%f", j, color[j].r, color[j].g, color[j].b, color[j].a);
				}
			}
		}
		// 面情報
		if (mesh->HasFaces()) {
			Log("\t\tFace:%d", mesh->mNumFaces);
			const aiFace* face = mesh->mFaces;
			for (u_int i = 0; i < mesh->mNumFaces; i++) {
				assert(2 <= face[i].mNumIndices && face[i].mNumIndices <= 3);// AI_MAX_FACE_INDICES);
				const u_int* indices = face[i].mIndices;
				if (face[i].mNumIndices == 2) {
					Log("\t\t\tFaces(%d,%d):%d,%d", i, face[i].mNumIndices, indices[0], indices[1]);
				}
				else if (face[i].mNumIndices == 3) {
					Log("\t\t\tFaces(%d,%d):%d,%d,%d", i, face[i].mNumIndices, indices[0], indices[1], indices[2]);
				}
			}
		}
		// 骨情報
		if (mesh->HasBones()) {
			Log("\t\tBones:%d", mesh->mNumBones);
			aiBone **bones = mesh->mBones;
			for (u_int i = 0; i < mesh->mNumBones; i++) {
				auto bone = bones[i];
				Log("\t\t\t%d,%s", i, bone->mName.C_Str());
				Log("\t\t\tOffsetMatrix");
				Log("\t\t\t(a1,a2,a3,a4):(%f,%f,%f,%f)", bone->mOffsetMatrix.a1, bone->mOffsetMatrix.a2, bone->mOffsetMatrix.a3, bone->mOffsetMatrix.a4);
				Log("\t\t\t(b1,b2,b3,b4):(%f,%f,%f,%f)", bone->mOffsetMatrix.b1, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.b4);
				Log("\t\t\t(c1,c2,c3,c4):(%f,%f,%f,%f)", bone->mOffsetMatrix.c1, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.c4);
				Log("\t\t\t(d1,d2,d3,d4):(%f,%f,%f,%f)", bone->mOffsetMatrix.d1, bone->mOffsetMatrix.d2, bone->mOffsetMatrix.d3, bone->mOffsetMatrix.d4);

				Log("\t\t\tWeights:%d", bone->mNumWeights);
				const aiVertexWeight *w = bone->mWeights;
				for (u_int j = 0; j < bone->mNumWeights; j++) {
					Log("\t\t\tweight(%d):%d,%f", j, w->mVertexId, w->mWeight);
				}
			}
		}

	};

	//Node -------------------------------------------------------------------------------------------
	std::function<const aiNode *(const aiNode *)> nodeFunc = [&](const aiNode *node)->aiNode * {
		Log("nodeName:%s", node->mName.C_Str());

		Log("\tnumMeshes:%d", node->mNumMeshes);
		for (u_int i = 0; i < node->mNumMeshes; i++) {
			Log("\t\tmeshIdx(%d):%d", i, node->mMeshes[i]);
			const struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			// Mesh information.
			meshFunc(mesh);
		}
		//const aiMesh* mesh = scene->HasMeshes[node->mMeshes[0]];
		//node->mTransformation
		//node->mMeshes
		Log("\tnumNodeChildren:%d", node->mNumChildren);
		for (u_int i = 0; i < node->mNumChildren; i++) {
			nodeFunc(node->mChildren[i]);
		}
		return nullptr;
	};

	nodeFunc(scene->mRootNode);

	//Materials --------------------------------------------------------------------------------------
	for (u_int i = 0; i < scene->mNumMaterials; i++) {
		auto material = scene->mMaterials[i];

		aiString path;
		material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), path);
		Log("TextureName(%d):%s", i, path.C_Str());

		{
			aiColor3D color;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			Log("ColorDiffuse(RGB):%f,%f,%f", color.r, color.g, color.b);
		}

		{
			aiColor3D color;
			material->Get(AI_MATKEY_COLOR_AMBIENT, color);
			Log("ColorAmbient(RGB):%f,%f,%f", color.r, color.g, color.b);
		}

		{
			aiColor3D color;
			material->Get(AI_MATKEY_COLOR_SPECULAR, color);
			Log("ColorSpecular(RGB):%f,%f,%f", color.r, color.g, color.b);
		}
		{
			float shininess = 80.0f;
			float strength = 0.0f;
			material->Get(AI_MATKEY_SHININESS, shininess);
			material->Get(AI_MATKEY_SHININESS_STRENGTH, strength);
			Log("Shininess,ShininessStrength:%f,%f", shininess, strength);
		}

		{
			aiColor3D color;
			material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
			Log("ColorEmissive(RGB):%f,%f,%f", color.r, color.g, color.b);
		}

		{
			int wireFrame = 0;
			int fillMode = GL_FILL;
			if (AI_SUCCESS == material->Get(AI_MATKEY_ENABLE_WIREFRAME, wireFrame)) {
				fillMode = wireFrame ? GL_LINE : GL_FILL;
			}
			//glPolygonMode(GL_FRONT_AND_BACK, fill_mode);
			Log("EnableWireframe:%d", fillMode, wireFrame);
		}

		{
			int two_sided = 0;
			aiReturn ret = material->Get(AI_MATKEY_TWOSIDED, two_sided);
			bool enable = (ret == AI_SUCCESS && two_sided);
			//if (enabled) glEnable(GL_CULL_FACE);
			//else glDisable(GL_CULL_FACE);
			Log("EnableCullFace, TwoSided:%d,%d", enable, two_sided);
		}
	}

	//Animations -------------------------------------------------------------------------------------
	Log("Animation:%d", scene->HasAnimations());
	if (scene->HasAnimations()) {
		for (u_int i = 0; i < scene->mNumAnimations; i++) {
			auto anim = scene->mAnimations[i];
			Log("\t%s", anim->mName.C_Str());

			// 階層アニメーション
			Log("\tNodeAnimation:%d", anim->mNumChannels);

			//aiNodeAnim** node_anim = anim->mChannels;
			for (u_int i = 0; i < anim->mNumChannels; ++i) {
				auto node_anim = anim->mChannels[i];
				Log("\t\tNodeName:%s", node_anim->mNodeName.C_Str());
				// 平行移動
				Log("\t\t\tPositionKey");
				for (u_int i = 0; i < node_anim->mNumPositionKeys; ++i) {
					auto pos = node_anim->mPositionKeys[i];
					Log("\t\t\t%d:%f:%f,%f,%f", i, pos.mTime, pos.mValue.x, pos.mValue.y, pos.mValue.z);
				}
				Log("\t\t\tScalingKey");
				// スケーリング
				for (u_int i = 0; i < node_anim->mNumScalingKeys; ++i) {
					auto scale = node_anim->mScalingKeys[i];
					Log("\t\t\t%d:%f:%f,%f,%f", i, scale.mTime, scale.mValue.x, scale.mValue.y, scale.mValue.z);
				}
				Log("\t\t\tRotationKey");
				// 回転
				for (u_int i = 0; i < node_anim->mNumRotationKeys; ++i) {
					auto rotation = node_anim->mRotationKeys[i];
					Log("\t\t\t%d:%f:%f,%f,%f", i, rotation.mTime, rotation.mValue.x, rotation.mValue.y, rotation.mValue.z);
				}
			}
		}
	}
}
