#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MESH.h"
#include "SHADER.h"

#include <stb_image.h>

#include <vector>
#include <string>

class Model {
public:
	Model(char *path) {
		loadModel(path);
	}

	void draw(Shader &shader);

private:

	vector<Texture> textures_loaded;

	vector<Mesh> meshes;
	string directory;

	void loadModel(string path);

	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

};