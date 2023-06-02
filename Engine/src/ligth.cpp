#include <ligth.hpp>

Ligth::Ligth(glm::vec3 color,glm::vec3 position):
ligthColor{color},
ligthPosition{position},
ligthCube{Cube(color,position)}
{}

void Ligth::setColorUniform(unsigned int programId,const std::string& name)
{
    glUniform3fv(glGetUniformLocation(programId,name.c_str()), 1, glm::value_ptr(ligthColor));
}

void Ligth::setPositionUniform(unsigned int programId,const std::string& name)
{
    glUniform3fv(glGetUniformLocation(programId,name.c_str()), 1, glm::value_ptr(ligthPosition)); 
}

void Ligth::setPosition(glm::vec3 newPosition)
{
    ligthPosition = newPosition;
}


glm::mat4 Ligth::getModelToWorldTransformationMatrix()
{
    return ligthCube.getModelToWorldTransformationMatrix();
}
void Ligth::draw()
{
    ligthCube.draw();
}