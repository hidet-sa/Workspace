
#include "Common.h"
#include "Logger.h"
#include "Dump.h"

//glfw & opengl
#include <GLFW/glfw3.h>
#include <gl/GLU.h>
#include <gl/GL.h>

//assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/cimport.h>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

//stb image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//モデルデータ
static std::string modelPath = "models/OBJ/spider.obj";

#include <map>
std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
GLuint *textureIds = nullptr;							// pointer to texture Array

//openGL
void InitialOpenGL();
void DrawOpenGL();
void ReSizeGLScene(GLsizei width, GLsizei height);
void FinalOpenGL();

//モデル
const aiScene *scene = nullptr;
const aiScene* LoadModel(std::string& filename);//モデルを読み込む。
void DrawModel(const aiScene *scene);
void RemoveModel(const aiScene *scene);
bool LoadGLTextures(const aiScene* scene);
void RemoveGLTextures();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	auto window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (window == nullptr) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	InitialOpenGL();
	ReSizeGLScene(640, 480);

	//assimp log 初期化。
	auto logger = LoggerManager::getInstance();
	// Now I am ready for logging my stuff
	logger->info("this is my info-call");

	//load model.
	scene = LoadModel(modelPath);

	LoadGLTextures(scene);

	//dump
	DumpModel(scene);

	while (!glfwWindowShouldClose(window))
	{
		/*
		** ここで OpenGL による描画を行う
		*/
		DrawOpenGL();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	RemoveModel(scene);

	RemoveGLTextures();

	FinalOpenGL();

	glfwDestroyWindow(window);
	glfwTerminate();

	//assimp log終了化。
	logger->purge();

	return EXIT_SUCCESS;
}

void InitialOpenGL()
{
	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[] = { 0.0f, 0.0f, 15.0f, 1.0f };

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

void DrawOpenGL()
{
	static GLfloat xrot = 0.0f;
	static GLfloat yrot = 0.0f;
	static GLfloat zrot = 0.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset MV Matrix

	glTranslatef(0.0f, -10.0f, -40.0f);	// Move 40 Units And Into The Screen

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glRotatef(zrot, 0.0f, 0.0f, 1.0f);

	DrawModel(scene);

	//xrot+=0.3f;
	yrot += 0.1f;
	//zrot+=0.4f;
}

void ReSizeGLScene(GLsizei width, GLsizei height)
{
	float fovy = 45.0f;
	float aspect = (float)width / height;
	float nearValue = 0.1f;
	float farValue = 100.0f;
	
	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();							// Reset The Projection Matrix
											// Calculate The Aspect Ratio Of The Window

	//	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	double scale = nearValue * tanf(fovy * 0.5f * 3.1415926536 / 180.0f);
	double x = width * scale * aspect / width;
	double y = height * scale / height;
	glFrustum(-x, x, -y, y, nearValue, farValue);

	glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
	glLoadIdentity();							// Reset The Modelview Matrix
}

void FinalOpenGL()
{
}

const aiScene *LoadModel(std::string& filename)
{
	auto logger = LoggerManager::getInstance();
	Assimp::Importer importer;

	// Check if file exists
	std::ifstream fin(filename.c_str());
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		logger->info(importer.GetErrorString());
		return nullptr;
	}

	auto scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
	scene = importer.GetOrphanedScene();//impoterからsceneを切り離す。

	// If the import failed, report it
	if (!scene)
	{
		logger->info(importer.GetErrorString());
		return nullptr;
	}

	// Now we can access the file's contents.
	logger->info("Import of scene " + filename + " succeeded.");

	return scene;
}

std::string getBasePath(const std::string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (std::string::npos == pos) ? "" : path.substr(0, pos + 1);
}

bool LoadGLTextures(const aiScene* scene)
{
	if (scene->HasTextures()) {
		return false;
	}

	/* getTexture Filenames and Numb of Textures */
	for (unsigned int m = 0; m<scene->mNumMaterials; m++)
	{
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS)
		{
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}

	int numTextures = textureIdMap.size();

	/* create and fill array with GL texture ids */
	textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds); /* Texture name generation */
											/* get iterator */
	std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();

	std::string basepath = getBasePath(modelPath);
	for (int i = 0; i<numTextures; i++)
	{
		//save IL image ID
		std::string filename = (*itr).first;  // get filename
		(*itr).second = &textureIds[i];	  // save texture id for filename in map
		itr++;								  // next texture

		std::string fileloc = basepath + filename;	/* Loading of image */

		int x, y, n;
		unsigned char *data = stbi_load(fileloc.c_str(), &x, &y, &n, STBI_rgb_alpha);

		if (nullptr != data) {
			// Binding of texture name
			glBindTexture(GL_TEXTURE_2D, textureIds[i]);
			// redefine standard texture values
			// We will use linear interpolation for magnification filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// We will use linear interpolation for minifying filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Texture specification
			glTexImage2D(GL_TEXTURE_2D, 0, n, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);// Texture specification.

			// we also want to be able to deal with odd texture dimensions
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
			stbi_image_free(data);
		}
		else
		{
			/* Error occurred */
			assert(0);
			//return false;
		}
	}
	return true;
}

void RemoveGLTextures()
{
	if (textureIdMap.size() > 0) {

		glDeleteTextures(textureIdMap.size(), textureIds);
		if (textureIds) {
			delete[] textureIds;
			textureIds = NULL;
		}
		textureIdMap.clear(); //no need to delete pointers in it manually here. (Pointers point to textureIds deleted in next step)
	}
}

void DrawModel(const aiScene *scene)
{
	std::function<void(const aiColor4D *)> Color4f = [](const aiColor4D *color)
	{
		glColor4f(color->r, color->g, color->b, color->a);
	};

	std::function<void(float [4], float, float, float, float)> set_float4 = [](float f[4], float a, float b, float c, float d)
	{
		f[0] = a;
		f[1] = b;
		f[2] = c;
		f[3] = d;
	};

	std::function<void(const aiColor4D *, float *)> color4_to_float4 = [](const aiColor4D *c, float *f)
	{
		f[0] = c->r;
		f[1] = c->g;
		f[2] = c->b;
		f[3] = c->a;
	};

	std::function<void(const aiMaterial *)> apply_material = [&](const aiMaterial *mtl)
	{
		float c[4];

		GLenum fill_mode;
		int ret1, ret2;
		aiColor4D diffuse;
		aiColor4D specular;
		aiColor4D ambient;
		aiColor4D emission;
		ai_real shininess, strength;
		int two_sided;
		int wireframe;
		unsigned int max;	// changed: to unsigned

		int texIndex = 0;
		aiString texPath;	//contains filename of texture

		if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
		{
			//bind texture;
			unsigned int texId = *textureIdMap[texPath.data];
			glBindTexture(GL_TEXTURE_2D, texId);
		}

		set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			color4_to_float4(&diffuse, c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
			color4_to_float4(&specular, c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

		set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
			color4_to_float4(&ambient, c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
			color4_to_float4(&emission, c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

		max = 1;
		ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
		else {
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
			set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
		}

		max = 1;
		if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
			fill_mode = wireframe ? GL_LINE : GL_FILL;
		else
			fill_mode = GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

		max = 1;
		if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);

		return;
	};

	std::function<void(const struct aiScene *, const struct aiNode *, float)> recursive_render = [&](const struct aiScene *sc, const struct aiNode *nd, float scale)
	{
		unsigned int i;
		unsigned int n = 0, t;
		aiMatrix4x4 m = nd->mTransformation;

		aiMatrix4x4 m2;
		aiMatrix4x4::Scaling(aiVector3D(scale, scale, scale), m2);
		m = m * m2;

		// update transform
		m.Transpose();
		glPushMatrix();
		glMultMatrixf((float*)&m);

		// draw all meshes assigned to this node
		for (; n < nd->mNumMeshes; ++n)
		{
			const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

			apply_material(sc->mMaterials[mesh->mMaterialIndex]);

			if (mesh->mNormals == NULL)
			{
				glDisable(GL_LIGHTING);
			}
			else
			{
				glEnable(GL_LIGHTING);
			}

			if (mesh->mColors[0] != NULL)
			{
				glEnable(GL_COLOR_MATERIAL);
			}
			else
			{
				glDisable(GL_COLOR_MATERIAL);
			}

			for (t = 0; t < mesh->mNumFaces; ++t) {
				const struct aiFace* face = &mesh->mFaces[t];
				GLenum face_mode;

				switch (face->mNumIndices)
				{
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
				}

				glBegin(face_mode);

				for (i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
				{
					int vertexIndex = face->mIndices[i];	// get group index for current index
					if (mesh->mColors[0] != NULL)
						Color4f(&mesh->mColors[0][vertexIndex]);
					if (mesh->mNormals != NULL)

						if (mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
						{
							glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
						}

					glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
				}
				glEnd();
			}
		}

		// draw all children
		for (n = 0; n < nd->mNumChildren; ++n) {
			recursive_render(sc, nd->mChildren[n], scale);
		}

		glPopMatrix();
	};

	recursive_render(scene, scene->mRootNode, 0.5);
}

void RemoveModel(const aiScene *scene)
{
	aiReleaseImport(scene);
}
