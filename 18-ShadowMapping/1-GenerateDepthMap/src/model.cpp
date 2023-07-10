#include <model.hpp>

#include <stb_image.hpp>

Model::Model(const char *path):
numberOfBones{0}
{
    loadModel(path);
}

void Model::Draw(Shader& shader)
{
    for(unsigned int i = 0; i < meshes.size();i++)
    {
        meshes[i].Draw(shader);
    }
}

glm::mat4 assimpMat4toGlm(const aiMatrix4x4& mat);

void Model::loadModel(std::string path)
{
    const aiScene* scene = import.ReadFile(
        path,
        aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType           | 
		aiProcess_Triangulate           | 
		aiProcess_GenSmoothNormals      | 
		aiProcess_FlipUVs               |
		aiProcess_LimitBoneWeights      |
        aiProcess_CalcTangentSpace
    );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ASSIMP::ERROR::" << import.GetErrorString() << std::endl;
        return;
    }

    assimpScene = scene;

    directory = path.substr(0, path.find_last_of('/'));
    
    /* INIT THE TRANSFORM MATRICES */
    globalTransformation = assimpMat4toGlm(scene->mRootNode->mTransformation);
    globalInverseTransformation = glm::inverse(globalTransformation);
    
    processNode(scene->mRootNode,scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes;i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh,scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren;i++)
    {
        processNode(node->mChildren[i],scene);
    }
}

void addBoneData(std::vector<Vertex>& vertices, unsigned int vertexIndex,unsigned int boneIndex, float weigth)
{
    /* INSERTING ID AND WEIGTH ON THE FIRST AVAILABLE SPOT */
    for(unsigned int p = 0; p < 4 ; p++)
    {
        if (vertices[vertexIndex].m_Weights[p] == 0.0f)
        {
            vertices[vertexIndex].m_BoneIDs[p] = boneIndex;
            vertices[vertexIndex].m_Weights[p] = weigth;
            return;
        }
    }

    assert(0);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    /* VERTEX POSITIONS */
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v;

        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        /* NORMALS */
        if(mesh->HasNormals())
        {
            v.normal.x = mesh->mNormals[i].x;
            v.normal.y = mesh->mNormals[i].y;
            v.normal.z = mesh->mNormals[i].z;
        }
        else
        {
            v.normal = glm::vec3(0.0f,0.0f,0.0f);
        }

        /* TANGENTS & BITANGENTS */
        if(mesh->HasTangentsAndBitangents())
        {
            /* TANGENT */
            v.tangent.x = mesh->mTangents[i].x;
            v.tangent.y = mesh->mTangents[i].y;
            v.tangent.z = mesh->mTangents[i].z;

            /* BITANGENT */
            v.bitangent.x = mesh->mBitangents[i].x;
            v.bitangent.y = mesh->mBitangents[i].y;
            v.bitangent.z = mesh->mBitangents[i].z;
        }
        else
        {
            v.tangent = glm::vec3(0.0f,0.0f,0.0f);
            v.bitangent = glm::vec3(0.0f,0.0f,0.0f);
        }

        /* TEXTURE COORDINATES */
        if(mesh->mTextureCoords[0])
        {
            v.textureCoordinates.x = mesh->mTextureCoords[0][i].x;
            v.textureCoordinates.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            v.textureCoordinates.x = 0.0f;
            v.textureCoordinates.y = 0.0f;
        }

        vertices.push_back(v);
    }

    if(mesh->HasBones())
    {
        /* INITIALIZE EACH VERTEX'S BONE IDS AND WEIGHTS */
        for (unsigned int i = 0; i < vertices.size(); i++)
        {
            for(unsigned int j = 0; j < 4 ; j++)
            {
                vertices[i].m_BoneIDs[j] = 0;
                vertices[i].m_Weights[j] = 0.0f;
            }
        }

        for(unsigned int j = 0; j < mesh->mNumBones; j++)
        {
            /* INSERTING BONE INDEX TO THE MAP IF NOT PRESENT */
            unsigned int boneIndex = 0;

            std::string boneName(mesh->mBones[j]->mName.data);

            if (boneMapping.find(boneName) == boneMapping.end()) {

                boneIndex = numberOfBones;

                numberOfBones++;

                bones.push_back({});
            }
            else {
                boneIndex = boneMapping[boneName];
            }

            boneMapping[boneName] = boneIndex;

            /* OBTAINING LOCAL MATRIX */
            bones[boneIndex].meshToBone = assimpMat4toGlm(mesh->mBones[j]->mOffsetMatrix);

            /* VERTICES AFFECTED BY THIS BONE */
            for (unsigned int z = 0; z < mesh->mBones[j]->mNumWeights; z++) {

                unsigned int vertexIndex = mesh->mBones[j]->mWeights[z].mVertexId;                
                float weigth = mesh->mBones[j]->mWeights[z].mWeight;
                
                addBoneData(vertices,vertexIndex,boneIndex,weigth);
            }
        }
    }
    else
    {
        for (unsigned int i = 0; i < vertices.size(); i++)
        {
            for(unsigned int j = 0; j < 4 ; j++)
            {
                vertices[i].m_BoneIDs[j] = 0;
                vertices[i].m_Weights[j] = 0.0f;
            }
        }
    }

    /* VERTEX INDICES */
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }  

    /* TEXTURE  */
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        
        /* DIFFUSE */
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        /* SPECULAR */
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        /* NORMAL */
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        
        /* HEIGTH */
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < texturesLoaded.size();j++)
        {
            if(std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

glm::mat4 assimpMat4toGlm(const aiMatrix4x4& mat)
{
    /* GLM COLUMN MAJOR vs ASSIMP ROW MAJOR */
    glm::mat4 result;
    result[0] = glm::vec4(mat[0][0], mat[1][0], mat[2][0], mat[3][0]); /* FIRST COLUMN  */
    result[1] = glm::vec4(mat[0][1], mat[1][1], mat[2][1], mat[3][1]); /* SECOND COLUMN */
    result[2] = glm::vec4(mat[0][2], mat[1][2], mat[2][2], mat[3][2]); /* THIRD  COLUMN */
    result[3] = glm::vec4(mat[0][3], mat[1][3], mat[2][3], mat[3][3]); /* FOURTH COLUMN */

    return result; 
}

glm::mat4 assimpMat4toGlm(const aiMatrix3x3& mat)
{
    glm::mat4 result;
    result[0] = glm::vec4(mat[0][0], mat[1][0], mat[2][0], 0.0f);
    result[1] = glm::vec4(mat[0][1], mat[1][1], mat[2][1], 0.0f);
    result[2] = glm::vec4(mat[0][2], mat[1][2], mat[2][2], 0.0f);
    result[3] = glm::vec4(0.0f     ,      0.0f,      0.0f, 1.0f);                       

    return result; 
}

void Model::boneTransform(float time, std::vector<glm::mat4>& ts)
{
    float ticksPerSecond = assimpScene->mAnimations[0]->mTicksPerSecond;
    float timeInTicks = time * ticksPerSecond;
    float animationTime = fmod(timeInTicks,assimpScene->mAnimations[0]->mDuration);

    readNodeHierarchy(animationTime,assimpScene->mRootNode,glm::mat4(1.0f));

    ts.resize(numberOfBones);

    for(unsigned int i = 0; i < numberOfBones; i++)
    {
        ts[i] = bones[i].finalTransformation;
    }
}

void Model::readNodeHierarchy(float animationTime, const aiNode* node, const glm::mat4& parentTransform)
{
	glm::mat4 identityTest = glm::mat4(1.0f);

	std::string nodeName(node->mName.data);

	const aiAnimation* animation = assimpScene->mAnimations[0];

    glm::mat4 nodeTransformation = assimpMat4toGlm(node->mTransformation);

	const aiNodeAnim* nodeAnim = nullptr;

	for (unsigned i = 0; i < animation->mNumChannels; i++) {
		const aiNodeAnim* nodeAnimationIndex = animation->mChannels[i];

		if (std::string(nodeAnimationIndex->mNodeName.data) == nodeName) {
			nodeAnim = nodeAnimationIndex;
		} 
	}

	if (nodeAnim) 
    {
		aiQuaternion rotationQuaternion;
        calculateInterpolatedRotation(rotationQuaternion,animationTime,nodeAnim);        
		glm::mat4 rotationMatrix = assimpMat4toGlm(rotationQuaternion.GetMatrix());
		
		aiVector3D translation;
		calculateInterpolatedTranslation(translation,animationTime,nodeAnim);

		glm::mat4 translationMatrix = glm::translate(rotationMatrix,glm::vec3(translation.x,translation.y,translation.z));

		nodeTransformation = translationMatrix;
	}
	
	glm::mat4 globalTransf = parentTransform * nodeTransformation;
	
	if (boneMapping.find(nodeName) != boneMapping.end()) {
		unsigned int boneIndex = boneMapping[nodeName];
		bones[boneIndex].finalTransformation = globalInverseTransformation  * globalTransf * bones[boneIndex].meshToBone;
	}

	for (unsigned i = 0; i < node->mNumChildren; i++) {
		readNodeHierarchy(animationTime, node->mChildren[i], globalTransf);
	}
}

void Model::calculateInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* nodeAnim)
{
	// we need at least two values to interpolate...
	if (nodeAnim->mNumRotationKeys == 1) {
		out = nodeAnim->mRotationKeys[0].mValue;
		return;
	}
	// Obtain the current rotation keyframe. 
	unsigned int rotationIndex = findRotation(animationTime,nodeAnim);

	// Calculate the next rotation keyframe and check bounds. 
	unsigned int nextRotationIndex = (rotationIndex + 1);
	assert(nextRotationIndex < nodeAnim->mNumRotationKeys);

	// Calculate delta time, i.e time between the two keyframes.
	float DeltaTime = nodeAnim->mRotationKeys[nextRotationIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime;

	// Calculate the elapsed time within the delta time.  
	float Factor = (animationTime - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);

	// Obtain the quaternions values for the current and next keyframe. 
	const aiQuaternion& StartRotationQ = nodeAnim->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& EndRotationQ = nodeAnim->mRotationKeys[nextRotationIndex].mValue;

	// Interpolate between them using the Factor. 
	aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);

	// Normalise and set the reference. 
	out = out.Normalize();
}

void Model::calculateInterpolatedTranslation(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim)
{
	// we need at least two values to interpolate...
	if (nodeAnim->mNumPositionKeys == 1) {
		out = nodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = findTranslation(animationTime,nodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < nodeAnim->mNumPositionKeys);
	float DeltaTime = nodeAnim->mPositionKeys[NextPositionIndex].mTime - nodeAnim->mPositionKeys[PositionIndex].mTime;
	float Factor = (animationTime - (float)nodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = nodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = nodeAnim->mPositionKeys[NextPositionIndex].mValue;

	aiVector3D Delta = End - Start;
	out = Start + Factor * Delta;
}

unsigned int Model::findRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
	// Check if there are rotation keyframes. 
	assert(nodeAnim->mNumRotationKeys > 0);

	// Find the rotation key just before the current animation time and return the index. 
	for (unsigned int i = 0; i < nodeAnim->mNumRotationKeys - 1; i++) {
		if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);

	return 0;
}

unsigned int Model::findTranslation(float animationTime, const aiNodeAnim* nodeAnim)
{
	assert(nodeAnim->mNumPositionKeys > 0);

	for (unsigned int i = 0; i < nodeAnim->mNumPositionKeys - 1; i++) {
		if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);

	return 0;
}

void Model::setBoneTransform(unsigned int index, const glm::mat4& ts,Shader& shader)
{
    shader.setMatrix4f("gBones["+ std::to_string(index)+"]",ts);
}