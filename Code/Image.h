#ifndef IMAGE_H
#define IMAGE_H
#pragma once

#include "SDL.h"
#include "config.h"

// Stores an image as an OpenGL texture
class Image
{
private:
	GLuint texture;
	GLfloat width;
	GLfloat height;
public:
	// surface must not be NULL.
	// ratio defines the resolution of the image in meters per pixel
	Image(SDL_Surface* surface, float ratio = PHYSICAL_RATIO);
	~Image() { }
	/* Creates an Image object based on the supplied file path and returns a
	 * pointer to the new Image. If the image could not be created, returns null.
	 */
	static Image* LoadFromFile(char* filePath);
	// Get the OpenGL texture for this Image
	GLuint GetTexture() const;
	// Get the default width of this Image in meters
	GLfloat GetWidth() const;
	// Get the default height of this Image in meters
	GLfloat GetHeight() const;
	// Draw the image
	// xPos and yPos define the center point in meters
	// theta defines the counterclockwise rotation in radians
	void Draw(GLfloat xPos, GLfloat yPos, GLfloat theta = 0.0f);
	// Draw the image
	// w and h define the dimensions to draw the image
	void Draw(GLfloat xPos, GLfloat yPos, GLfloat w, GLfloat h, GLfloat theta = 0.0f);
};

#endif