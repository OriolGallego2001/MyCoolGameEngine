#include "ModuleRenderExercise.h"
#include "ModuleOpenGL.h"
#include "Application.h"
#include "../glew-2.1.0/include/GL/glew.h"
#include "ModuleWindow.h"
#include "ModuleEditorCamera.h"
#include "TextureLoader.h"
#include "ModelLoader.h"

ModuleRenderExercise::ModuleRenderExercise()
{
    this->triangleVBO = 0;
    this->vertex_id = 0;
    this->fragment_id = 0;
    this->program_id = 0;
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}


bool ModuleRenderExercise::Init()
{
    triangleVBO = CreateTriangleVBO();

    vertex_id = CompileShader(GL_VERTEX_SHADER, LoadShaderSource("Data/Shaders/basic.vs"));
    fragment_id = CompileShader(GL_FRAGMENT_SHADER, LoadShaderSource("Data/Shaders/flat.fs"));
    program_id = CreateProgram(vertex_id, fragment_id);
    object = new ModelLoader();

    object->loadModel("Data/BakerHouse.gltf");
    

    return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
    return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
    // RenderVBO(this->triangleVBO, this->program_id);
    //RenderTriangle();
    //RenderMonkey();
    RenderModel();
    
    return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
    return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    return true;
}

unsigned ModuleRenderExercise::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
    unsigned program_id = glCreateProgram();
    glAttachShader(program_id, vtx_shader);
    glAttachShader(program_id, frg_shader);
    glLinkProgram(program_id);
    int res;
    glGetProgramiv(program_id, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetProgramInfoLog(program_id, len, &written, info);
            LOG("Program Log Info: %s", info);
            free(info);
        }
    }
    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    return program_id;

}

unsigned ModuleRenderExercise::CompileShader(unsigned type, const char* source)
{
    unsigned shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, 0);
    glCompileShader(shader_id);
    int res = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetShaderInfoLog(shader_id, len, &written, info);
            LOG("Log Info: %s", info);
            free(info);
        }
    }
    return shader_id;
}

char* ModuleRenderExercise::LoadShaderSource(const char* shader_file_name)
{
    char* data = nullptr;
    FILE* file = nullptr;
    fopen_s(&file, shader_file_name, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        data = (char*)malloc(size + 1);
        fseek(file, 0, SEEK_SET);
        fread(data, 1, size, file);
        data[size] = 0;
        fclose(file);
    }
    return data;

}

void ModuleRenderExercise::RenderVBO(unsigned vbo, unsigned program)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    // size = 3 float per vertex
    // stride = 0 is equivalent to stride = sizeof(float)*3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glUseProgram(program);
    // 1 triangle to draw = 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);

}

unsigned ModuleRenderExercise::CreateTriangleVBO()
{
    float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    unsigned vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

    return vbo;

}

void ModuleRenderExercise::DestroyTriangleVBO()
{
    glDeleteBuffers(1, &triangleVBO);

}

void ModuleRenderExercise::RenderTriangle()
{
    float4x4 viewproj = App->GetEditorCamera()->GetViewProjMatrix();
    float4x4 model = float4x4::identity;
    glUseProgram(program_id);
    glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_TRUE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program_id, "viewproj"), 1, GL_TRUE, &viewproj[0][0]);
    
    // TODO: bind buffer and vertex attributes
    RenderVBO(triangleVBO, program_id);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ModuleRenderExercise::RenderMonkey()
{

    
    float buffer_data[] = {
         -1.0f, -1.0f, 0.0f, // ← v0 pos
         1.0f, -1.0f, 0.0f, // ← v1 pos
         -1.0f, 1.0f, 0.0f, // ← v2 pos
         -1.0f, 1.0f, 0.0f, // ← v3 pos
         1.0f, -1.0f, 0.0f, // ← v4 pos
         1.0f, 1.0f, 0.0f,  // ← v5 pos



         0.0f, 1.0f, // ← v0 texcoord
         1.0f, 1.0f, // ← v1 texcoord
         0.0f, 0.0f, // ← v2 texcoord
         0.0f, 0.0f, // ← v3 texcoord
         1.0f, 1.0f, // ← v4 texcoord
         1.0f, 0.0f, // ← v5 texcoord
    };

    unsigned vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);

    float4x4 viewproj = App->GetEditorCamera()->GetViewProjMatrix();
    float4x4 model = float4x4::identity;

    glUseProgram(program_id);
    glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_TRUE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program_id, "viewproj"), 1, GL_TRUE, &viewproj[0][0]);//&App->GetEditorCamera()->ViewProjMatrix[0][0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
        (void*)0 // buffer offset
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
        (void*)(sizeof(float) * 3 * 6) // buffer offset
    );


    TextureLoader* mytex = new TextureLoader();
    mytex->LoadTexture(L"Data/Test-image-Baboon.ppm");
    glActiveTexture(GL_TEXTURE5);
    mytex->BindTexture();

    RenderVBO(vbo, program_id);

    glDrawArrays(GL_TRIANGLES, 0, 6);



}

void ModuleRenderExercise::RenderModel()
{
    float4x4 viewproj = App->GetEditorCamera()->GetViewProjMatrix();
    float4x4 model = float4x4::identity * 100.0f;
    model[3][3] = 1.0f;

    glUseProgram(program_id);
    glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_TRUE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program_id, "viewproj"), 1, GL_TRUE, &viewproj[0][0]);//&App->GetEditorCamera()->ViewProjMatrix[0][0]);

    object->Render(program_id);
}
