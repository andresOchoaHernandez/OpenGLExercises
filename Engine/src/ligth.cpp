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

void Ligth::move(glm::mat4 transformation)
{
    ligthCube.move(transformation);
    ligthPosition = ligthCube.getPosition();
}

glm::vec3 Ligth::getPosition()
{
    return ligthPosition;
}

void Ligth::updatePosition(glm::vec3 newPos)
{
    ligthCube.updatePosition(newPos);
    ligthPosition = ligthCube.getPosition();
}