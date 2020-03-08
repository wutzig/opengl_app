#include "header.h"

Renderer::Renderer(float width, float height):
    width(width), height(height){

    for(int row = 0; row < NUM_GRID; row++){//Create Terrain
        for(int col = 0; col < NUM_GRID; col++){
            float coordCol = col * TILE_SIZE - float(TILE_SIZE) / 2.0f;
            float coordRow = row * TILE_SIZE - float(TILE_SIZE) / 2.0f;
            
            this->AddTriplet(&this->terrain_vertices, coordRow + OFFSET, 0.0f, coordCol + OFFSET);
            this->AddTriplet(&this->terrain_vertices, coordRow + OFFSET, 0.0f, coordCol - OFFSET + TILE_SIZE);
            this->AddTriplet(&this->terrain_vertices, coordRow - OFFSET + TILE_SIZE, 0.0f, coordCol - OFFSET + TILE_SIZE);
            this->AddTriplet(&this->terrain_vertices, coordRow - OFFSET + TILE_SIZE, 0.0f, coordCol + OFFSET);

            for(int j = 0; j < 4; j++){//Terrain Colors
                this->AddTriplet(&this->terrain_colors, 0.4f, 0.5f, 0.2f);
                // this->AddTriplet(&this->terrain_normals, 0.0f, 1.0f, 0.0f);
            }
        }
    }

    {//Upload Terrain to OpenGL
        glCreateVertexArrays(1, &this->terrainVertexArrayID);
        glBindVertexArray(this->terrainVertexArrayID);
        
        uint buffer = 0;
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->terrain_vertices.size(), &(this->terrain_vertices[0]), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glCreateBuffers(1, &this->terrainColorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, this->terrainColorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->terrain_colors.size(), &(this->terrain_colors[0]), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glBindVertexArray(NULL);
    }

    std::vector<float> cube_normals;
    cube_normals.reserve(12 * 6);
    std::vector<float> brown;
    brown.reserve(12 * 6);
    std::vector<float> blue;
    blue.reserve(12 * 6);
    std::vector<float> green;
    blue.reserve(12 * 6);
    {//Make basic cube
        for(uint j = 0; j < 4; j++) this->AddTriplet(&cube_normals, 0.0f, -1.0f, 0.0f);
        for(uint j = 0; j < 4; j++) this->AddTriplet(&cube_normals, 0.0f, 1.0f, 0.0f);
        for(uint j = 0; j < 4; j++) this->AddTriplet(&cube_normals, -1.0f, 0.0f, 0.0f);
        for(uint j = 0; j < 4; j++) this->AddTriplet(&cube_normals, 1.0f, 0.0f, 0.0f);
        for(uint j = 0; j < 4; j++) this->AddTriplet(&cube_normals, 0.0f, 0.0f, -1.0f);
        for(uint j = 0; j < 4; j++) this->AddTriplet(&cube_normals, 0.0f, 0.0f, 1.0f);

        for(uint j = 0; j < 24; j++) this->AddTriplet(&brown, 0.7f, 0.4f, 0.2f);
        for(uint j = 0; j < 24; j++) this->AddTriplet(&blue, 0.2f, 0.4f, 0.7f);
        for(uint j = 0; j < 24; j++) this->AddTriplet(&green, 0.5f, 0.7f, 0.2f);
    }
    float vxmin = -0.2, vxmax =  0.2, vymin =  0.05, vymax =  0.65, vzmin = -0.2, vzmax =  0.2;
    std::vector<float> cube_vertices = Renderer::CubeVertices(vxmin, vxmax, vymin, vymax, vzmin, vzmax);
    std::vector<float> trunk_vertices = Renderer::CubeVertices(vxmin + 0.05, vxmax - 0.05, 0, vymax + 0.15, vzmin + 0.05, vzmax - 0.05);
    std::vector<float> leaf_vertices = Renderer::CubeVertices(vxmin - 0.1, vxmax + 0.1, vymax - 0.15, vymax + 0.75, vzmin - 0.1, vzmax + 0.1);

    std::vector<float> tree_vertices;
    tree_vertices.reserve(2 * 12 * 6);
    tree_vertices.insert(tree_vertices.end(), trunk_vertices.begin(), trunk_vertices.end());
    tree_vertices.insert(tree_vertices.end(), leaf_vertices.begin(), leaf_vertices.end());

    std::vector<float> tree_normals;
    tree_normals.reserve(2 * 12 * 6);
    tree_normals.insert(tree_normals.end(), cube_normals.begin(), cube_normals.end());
    tree_normals.insert(tree_normals.end(), cube_normals.begin(), cube_normals.end());

    std::vector<float> tree_colors;
    tree_colors.reserve(2 * 12 * 6);
    tree_colors.insert(tree_colors.end(), brown.begin(), brown.end());
    tree_colors.insert(tree_colors.end(), green.begin(), green.end());

    //make the player
    this->dynamicModels.emplace_back(6, &cube_vertices[0], &cube_normals[0], &blue[0]);
    this->dynamicModels[0].entities.emplace_back(glm::vec3(NUM_GRID / 2 * TILE_SIZE, 0.0f, NUM_GRID / 2 * TILE_SIZE));

    float positions[NUM_TREES];
    float angles[NUM_TREES];
    {//generate tree locations
        this->blockedTiles.reserve(NUM_TREES);
        double r = glfwGetTime() * 100000000.0f;
        r -= glm::floor(r);
        srand(100 * r);
        uint j = 0;
        while(j < NUM_TREES){
            int k = rand() % NUM_GRID;
            int i = rand() % NUM_GRID;
            float angle = float(rand() % 100)/100.0 * 2 * PI;
            if( this->blockedTiles.find(NUM_GRID * k + i) == this->blockedTiles.end() ){
                this->blockedTiles.emplace(NUM_GRID * k + i);
                positions[j] = float(NUM_GRID * k + i);
                angles[j] = angle;
                j++;
            }
        }
    }

    this->staticModels.emplace_back(12, &tree_vertices[0], &tree_normals[0], &tree_colors[0], NUM_TREES, positions, angles);

    {//Create GUI
        this->gui_vertices = {-0.9f, 0.6f, 0.0f,
                            -0.9f, 0.9f, 0.0f,
                            -0.6f, 0.9f, 0.0f,
                            -0.6f, 0.6f, 0.0f};
        
        glCreateVertexArrays(1, &this->guiVertexArrayID);
        glBindVertexArray(this->guiVertexArrayID);
        uint buffer = 0;
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, &this->gui_vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glBindVertexArray(NULL);
    }
    float center = float(NUM_GRID * TILE_SIZE) / 2.0f;
    glm::vec3 lookat = { center, 0.0f, center };
    this->camera = { INIT_CAMERA_PITCH, INIT_CAMERA_YAW, lookat };
       
    this->viewMatrix = glm::lookAt(this->camera.position, this->camera.lookat, glm::vec3(0.0f, 1.0f, 0.0f));
    this->invViewMatrix = glm::inverse(this->viewMatrix);

    this->UpdatePerspective(this->width, this->height);
    this->InitShadowMap();

    this->lightPos = glm::vec3(0.5f, 2.0f, 2.0f);

}

Renderer::~Renderer(){
    glDeleteVertexArrays(1, &this->terrainVertexArrayID);
    for(auto& model : this->staticModels){
        uint id = model.GetVertexArrayID();
        glDeleteVertexArrays(1, &id);
    }
    for(auto& model : this->dynamicModels){
        uint id = model.GetVertexArrayID();
        glDeleteVertexArrays(1, &id);
    }
}

std::vector<float> Renderer::CubeVertices(float vxmin, float vxmax, float vymin, float vymax, float vzmin, float vzmax){
    std::vector<float> cube_vertices;
    AddTriplet(&cube_vertices, vxmin, vymin, vzmin);
    AddTriplet(&cube_vertices, vxmax, vymin, vzmin);
    AddTriplet(&cube_vertices, vxmax, vymin, vzmax);
    AddTriplet(&cube_vertices, vxmin, vymin, vzmax);

    AddTriplet(&cube_vertices, vxmin, vymax, vzmin);
    AddTriplet(&cube_vertices, vxmin, vymax, vzmax);
    AddTriplet(&cube_vertices, vxmax, vymax, vzmax);
    AddTriplet(&cube_vertices, vxmax, vymax, vzmin);

    AddTriplet(&cube_vertices, vxmin, vymin, vzmin);
    AddTriplet(&cube_vertices, vxmin, vymin, vzmax);
    AddTriplet(&cube_vertices, vxmin, vymax, vzmax);
    AddTriplet(&cube_vertices, vxmin, vymax, vzmin);

    AddTriplet(&cube_vertices, vxmax, vymin, vzmin);
    AddTriplet(&cube_vertices, vxmax, vymax, vzmin);
    AddTriplet(&cube_vertices, vxmax, vymax, vzmax);
    AddTriplet(&cube_vertices, vxmax, vymin, vzmax);

    AddTriplet(&cube_vertices, vxmin, vymin, vzmin);
    AddTriplet(&cube_vertices, vxmin, vymax, vzmin);
    AddTriplet(&cube_vertices, vxmax, vymax, vzmin);
    AddTriplet(&cube_vertices, vxmax, vymin, vzmin);

    AddTriplet(&cube_vertices, vxmin, vymin, vzmax);
    AddTriplet(&cube_vertices, vxmax, vymin, vzmax);
    AddTriplet(&cube_vertices, vxmax, vymax, vzmax);
    AddTriplet(&cube_vertices, vxmin, vymax, vzmax);
        
    return cube_vertices;
}
void Renderer::AddTriplet(std::vector<float>* vertices, float x, float y, float z){
    vertices->push_back(x);
    vertices->push_back(y);
    vertices->push_back(z);
}
void Renderer::UpdateMousePos(double posX, double posY){
    this->mousePosX = uint(posX);
    this->mousePosY = uint(posY);
}
void Renderer::UpdatePerspective(float width, float height){
    this->width = width;
    this->height = height;
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), this->width / this->height, 0.1f, 100.0f);
    this->invProjectionMatrix = glm::inverse(this->projectionMatrix);
    this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;
}
void Renderer::UpdateView(float deltaX, float deltaY){
    this->camera.Rotate(deltaX / this->width * PI, deltaY / this->height * PI);
    this->viewMatrix = glm::lookAt(this->camera.position, this->camera.lookat, glm::vec3(0.0f, 1.0f, 0.0f));
    this->invViewMatrix = glm::inverse(this->viewMatrix);
    this->projectionViewMatrix = this->projectionMatrix * this->viewMatrix;
}
void Renderer::DrawScene(uint terrainShaderProgramID, uint staticShaderProgramID, uint dynamicShaderProgramID,
                         uint shadowStaticShaderProgramID, uint shadowDynamicShaderProgramID,
                         uint guiShaderProgramID, double delta_time){

    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-40.0f, 40.0f, -60.0f, 20.0f, -20.0f, 40.0f);
    glm::mat4 depthViewMatrix = glm::lookAt(this->lightPos, glm::vec3(0.0f), glm::vec3(0,1,0));
    this->depthProjectionViewMatrix = depthProjectionMatrix * depthViewMatrix;

    if( this->select_index >= 0 ){
        glm::vec3 goal_position = glm::vec3(
            this->terrain_vertices[12 * select_index     ] - OFFSET + float(TILE_SIZE) / 2.0f,
            0.0f,
            this->terrain_vertices[12 * select_index + 2 ] - OFFSET + float(TILE_SIZE) / 2.0f);

        Entity* entity = &this->dynamicModels[0].entities[0];
        glm::ivec2 goalIndex = entity->GetCurrentGoalIndex();
        if( goalIndex.x * NUM_GRID + goalIndex.y != this->select_index ){
            goalIndex.y = uint(this->select_index % NUM_GRID);
            goalIndex.x = (this->select_index - goalIndex.y) / NUM_GRID;
            entity->SetGoalIndex(goalIndex);
        }
        int sizeOfVertex = 12 * sizeof(float);
        uint index = entity->GetTileIndex().x * NUM_GRID + entity->GetTileIndex().y;
        glNamedBufferSubData(this->terrainColorBufferID, sizeOfVertex * index, sizeOfVertex, &dark_green[0]);
        entity->ChangePosition(delta_time, &this->blockedTiles);
        index = entity->GetTileIndex().x * NUM_GRID + entity->GetTileIndex().y;
        glNamedBufferSubData(this->terrainColorBufferID, sizeOfVertex * index, sizeOfVertex, &light_blue[0]);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferID);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    glUseProgram(shadowStaticShaderProgramID);
    glUniformMatrix4fv(this->shadowStaticDepthProjectionViewMatrixLoc, 1, GL_FALSE, &this->depthProjectionViewMatrix[0][0]);
    
    glCullFace(GL_FRONT);
    this->RenderTerrain();
    for(auto& model : this->staticModels){
        model.Bind();
        glDrawArraysInstanced(GL_QUADS, 0, 4 * model.GetNumQuads(), NUM_TREES);
        model.Unbind();
    }

    glUseProgram(shadowDynamicShaderProgramID);
    glUniformMatrix4fv(this->shadowDynamicDepthProjectionViewMatrixLoc, 1, GL_FALSE, &this->depthProjectionViewMatrix[0][0]);
    for(auto& model : this->dynamicModels){
        model.Bind();
        for(auto& entity : model.entities){
            glUniformMatrix4fv(this->shadowDynamicTransformationMatrixLoc, 1, GL_FALSE, entity.GetTransformationMatrix());
            glDrawArrays( GL_QUADS, 0, 12 * model.GetNumQuads());
        }
        model.Unbind();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    glViewport(0, 0, this->width, this->height);
    glBindTexture(GL_TEXTURE_2D, this->depthTextureID);
    glCullFace(GL_BACK);

    // glUseProgram(guiShaderProgramID);
    // glBindVertexArray(this->guiVertexArrayID);
    // glEnableVertexAttribArray(0);
    // glDrawArrays(GL_QUADS, 0, 12);
    // glDisableVertexAttribArray(0);

    glUseProgram(terrainShaderProgramID);
    glUniformMatrix4fv(this->terrainProjectionViewMatrixLoc, 1, GL_FALSE, &this->projectionViewMatrix[0][0]);
    glUniformMatrix4fv(this->terrainDepthProjectionViewMatrixLoc, 1, GL_FALSE, &this->depthProjectionViewMatrix[0][0]);
    this->RenderTerrain();
    
    glUseProgram(staticShaderProgramID);
    glUniform3fv(this->staticLightPositionLoc, 1, &this->lightPos[0]);
    glUniformMatrix4fv(this->staticProjectionViewMatrixLoc, 1, GL_FALSE, &this->projectionViewMatrix[0][0]);
    glUniformMatrix4fv(this->staticDepthProjectionViewMatrixLoc, 1, GL_FALSE, &this->depthProjectionViewMatrix[0][0]);
    this->RenderTerrain();
    for(auto& model : this->staticModels){
        model.Bind();
        glDrawArraysInstanced(GL_QUADS, 0, 4 * model.GetNumQuads(), NUM_TREES);
        model.Unbind();
    }

    glUseProgram(dynamicShaderProgramID);
    glUniform3fv(this->dynamicLightPositionLoc, 1, &this->lightPos[0]);
    glUniformMatrix4fv(this->dynamicProjectionViewMatrixLoc, 1, GL_FALSE, &this->projectionViewMatrix[0][0]);
    glUniformMatrix4fv(this->dynamicDepthProjectionViewMatrixLoc, 1, GL_FALSE, &this->depthProjectionViewMatrix[0][0]);
    for(auto& model : this->dynamicModels){
        model.Bind();
        for(auto& entity : model.entities){
            glUniformMatrix4fv(this->dynamicTransformationMatrixLoc, 1, GL_FALSE, entity.GetTransformationMatrix());
            glDrawArrays( GL_QUADS, 0, 12 * model.GetNumQuads());
        }
        model.Unbind();
    }
    glBindVertexArray(NULL);
    glUseProgram(NULL);
}

void Renderer::RenderTerrain(){
    glBindVertexArray(this->terrainVertexArrayID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
 
    glDrawArrays( GL_QUADS, 0, this->terrain_vertices.size() / 3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(NULL);
}
void Renderer::InitShadowMap(){
    glGenFramebuffers(1, &this->frameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferID);
    std::cout<<"Framebuffer ID: "<<this->frameBufferID<<std::endl;
    glCreateTextures(GL_TEXTURE_2D, 1, &this->depthTextureID);
    glBindTexture(GL_TEXTURE_2D, this->depthTextureID);
    std::cout<<"Depth Texture ID: "<<this->depthTextureID<<std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, 
                    0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureID, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout<<"Framebuffer not initialized!";
        glDeleteTextures(1, &depthTextureID);
        glDeleteBuffers(1, &this->frameBufferID);
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}
void Renderer::GetUniformLocations(uint terrainShaderProgramID, 
                                   uint staticShaderProgramID, uint dynamicShaderProgramID, 
                                   uint shadowStaticShaderProgramID, uint shadowDynamicShaderProgramID){
    this->terrainProjectionViewMatrixLoc = glGetUniformLocation(terrainShaderProgramID, "projectionViewMatrix");
    this->terrainDepthProjectionViewMatrixLoc = glGetUniformLocation(terrainShaderProgramID, "depthProjectionViewMatrix");
    
    this->staticProjectionViewMatrixLoc = glGetUniformLocation(staticShaderProgramID, "projectionViewMatrix");
    this->staticDepthProjectionViewMatrixLoc = glGetUniformLocation(staticShaderProgramID, "depthProjectionViewMatrix");
    this->staticLightPositionLoc = glGetUniformLocation(staticShaderProgramID, "lightPos");

    this->dynamicProjectionViewMatrixLoc = glGetUniformLocation(dynamicShaderProgramID, "projectionViewMatrix");
    this->dynamicDepthProjectionViewMatrixLoc = glGetUniformLocation(dynamicShaderProgramID, "depthProjectionViewMatrix");
    this->dynamicLightPositionLoc = glGetUniformLocation(dynamicShaderProgramID, "lightPos");
    this->dynamicTransformationMatrixLoc = glGetUniformLocation(dynamicShaderProgramID, "transformationMatrix");
    
    this->shadowStaticDepthProjectionViewMatrixLoc = glGetUniformLocation(shadowStaticShaderProgramID, "depthProjectionViewMatrix");

    this->shadowDynamicDepthProjectionViewMatrixLoc = glGetUniformLocation(shadowDynamicShaderProgramID, "depthProjectionViewMatrix");
    this->shadowDynamicTransformationMatrixLoc = glGetUniformLocation(shadowDynamicShaderProgramID, "transformationMatrix");
}
glm::vec3 Renderer::quatRot(const glm::vec3& point, const glm::vec3& axis, double angle, const glm::vec3& origin){
    glm::vec3 r = float(glm::sin(angle / 2.0)) * axis;
    glm::vec3 P = point - origin;
    return point + glm::cross( 2.0f * r, glm::cross(r, P) + float(glm::cos(angle / 2.0)) * P );
    return r;
}

void Renderer::ShootRay(){
    //compute the device coordinates of the mouse position
    float mouseDeviceX = 2.0f * float(this->mousePosX) / this->width - 1.0f;
    float mouseDeviceY = 1.0f - 2.0f * float(this->mousePosY) / this->height;
    glm::vec4 rayEyeSpace = this->invProjectionMatrix * glm::vec4(mouseDeviceX, mouseDeviceY, -1.0f, 1.0f);
    rayEyeSpace = this->invViewMatrix * glm::vec4(rayEyeSpace.x, rayEyeSpace.y, -1.0f, 0.0f);
    glm::vec3 rayDirection = glm::normalize(glm::vec3(rayEyeSpace.x, rayEyeSpace.y, rayEyeSpace.z));

    float cos_ray_normal = rayDirection.y;
    float length = TILE_SIZE - 2 * OFFSET;
    int active_index = -1;
    for(uint row = 0; row < NUM_GRID; row++){
        for(uint col = 0; col < NUM_GRID; col++){
            uint index = col * NUM_GRID + row;
            if(this->blockedTiles.find(index) != blockedTiles.end()) continue;
            glm::vec3 orig_to_square = glm::vec3(this->terrain_vertices[12 * index], this->terrain_vertices[12 * index + 1], this->terrain_vertices[12 * index + 2]) - this->camera.position;
            if(orig_to_square.y / cos_ray_normal >= 0.0){
                float sigma = -glm::dot(glm::vec3(0.0f, length * orig_to_square.z, -length * orig_to_square.y), rayDirection) / cos_ray_normal;
                if(sigma >= 0.0 && sigma <= length){
                    float theta = glm::dot(glm::vec3(length * orig_to_square.y, -length * orig_to_square.x, 0.0f), rayDirection) / cos_ray_normal;
                    if(theta >= 0.0 && theta <= length){
                        active_index = index;
                        break;
                    }
                }
            }
        }
        if(active_index >= 0) break;
    }
    if(this->select_index != active_index){
        if(this->select_index >= 0){
            glNamedBufferSubData(this->terrainColorBufferID, 12 * this->select_index * sizeof(float), 12 * sizeof(float), &dark_green[0]);
        }
        if(active_index >= 0){
            glNamedBufferSubData(this->terrainColorBufferID, 12 * active_index * sizeof(float), 12 * sizeof(float), &light_green[0]);
        }
        this->select_index = active_index;
    }
}