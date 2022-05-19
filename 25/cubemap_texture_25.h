#pragma once
#ifndef CUBEMAP_H
#define	CUBEMAP_H

#include <string>
#include <GL/glew.h>
#include <Magick++.h>

using namespace std;

class CubemapTexture //реализацию кубической текстуры и предоставляет простой интерфейс для ее загрузки и использования
{
public:
//принимает директорию и 6 имен файлов, которые содержат стороны куба
    CubemapTexture(const string& Directory,
        const string& PosXFilename,
        const string& NegXFilename,
        const string& PosYFilename,
        const string& NegYFilename,
        const string& PosZFilename,
        const string& NegZFilename);

    ~CubemapTexture();

    bool Load();//загрузка изображений

    void Bind(GLenum TextureUnit);//текстура для куба

private:

    string m_fileNames[6];
    GLuint m_textureObj;
};

#endif	/* CUBEMAP_H */
