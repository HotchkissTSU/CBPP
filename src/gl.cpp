#include "cbpp/cbpp_gl.h"

#ifdef _WIN64
#include <windows.h>
#endif

#ifdef __linux__
void* _linux_libGL_handle = nullptr;
#endif

#include <stdio.h>

#ifdef _WIN64
template<typename T> bool cb_GetProcAddress_(T& ptr_ref, const char* func_name) {
	ptr_ref = (T)wglGetProcAddress(func_name);
	if(ptr_ref == 0 ||
		(ptr_ref == (void*)0x1) || (ptr_ref == (void*)0x2) || (ptr_ref == (void*)0x3) ||
		(ptr_ref == (void*)-1) ){
			HMODULE module = LoadLibraryA("opengl32.dll");
			ptr_ref = (T)GetProcAddress(module, func_name);
		}

	FILE* fl = fopen("logs/OpenGL_load.txt", "a");
	fprintf(fl, "%s: %x\n", func_name, ptr_ref);
	fclose(fl);
	
	return (ptr_ref != (void*)0) && (ptr_ref != (void*)0x1) && (ptr_ref != (void*)0x2) && (ptr_ref != (void*)0x3) && (ptr_ref != (void*)-1);
}
#endif

#ifdef __linux__
template<typename T> bool cb_GetProcAddress_(T& ptr_ref, const char* func_name) {

	ptr_ref = (T)dlsym(_linux_libGL_handle, func_name);
	//printf("%s = %x\n", func_name, ptr_ref);
	FILE* fl = fopen("logs/OpenGL_load.txt", "a");
	fprintf(fl, "%s: %x\n", func_name, ptr_ref);
	fclose(fl);
	
	return (ptr_ref != (void*)0) && (ptr_ref != (void*)0x1) && (ptr_ref != (void*)0x2) && (ptr_ref != (void*)0x3) && (ptr_ref != (void*)-1);
}
#endif

/* ====================================
 * OpenGL 1.0
 */
#ifdef GL_VERSION_1_0
void (GLAPIENTRY* glCullFace)(GLenum mode);
void (GLAPIENTRY* glFrontFace)(GLenum mode);
void (GLAPIENTRY* glHint)(GLenum target, GLenum mode);
void (GLAPIENTRY* glLineWidth)(GLfloat width);
void (GLAPIENTRY* glPointSize)(GLfloat size);
void (GLAPIENTRY* glPolygonMode)(GLenum face, GLenum mode);
void (GLAPIENTRY* glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
void (GLAPIENTRY* glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
void (GLAPIENTRY* glTexParameterfv)(GLenum target, GLenum pname, const GLfloat* params);
void (GLAPIENTRY* glTexParameteri)(GLenum target, GLenum pname, GLint param);
void (GLAPIENTRY* glTexParameteriv)(GLenum target, GLenum pname, const GLint* params);
void (GLAPIENTRY* glTexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glDrawBuffer)(GLenum buf);
void (GLAPIENTRY* glClear)(GLbitfield mask);
void (GLAPIENTRY* glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (GLAPIENTRY* glClearStencil)(GLint s);
void (GLAPIENTRY* glClearDepth)(GLdouble depth);
void (GLAPIENTRY* glStencilMask)(GLuint mask);
void (GLAPIENTRY* glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void (GLAPIENTRY* glDepthMask)(GLboolean flag);
void (GLAPIENTRY* glDisable)(GLenum cap);
void (GLAPIENTRY* glEnable)(GLenum cap);
void (GLAPIENTRY* glFinish)(void);
void (GLAPIENTRY* glFlush)(void);
void (GLAPIENTRY* glBlendFunc)(GLenum sfactor, GLenum dfactor);
void (GLAPIENTRY* glLogicOp)(GLenum opcode);
void (GLAPIENTRY* glStencilFunc)(GLenum func, GLint ref, GLuint mask);
void (GLAPIENTRY* glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
void (GLAPIENTRY* glDepthFunc)(GLenum func);
void (GLAPIENTRY* glPixelStoref)(GLenum pname, GLfloat param);
void (GLAPIENTRY* glPixelStorei)(GLenum pname, GLint param);
void (GLAPIENTRY* glReadBuffer)(GLenum src);
void (GLAPIENTRY* glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
void (GLAPIENTRY* glGetBooleanv)(GLenum pname, GLboolean* data);
void (GLAPIENTRY* glGetDoublev)(GLenum pname, GLdouble* data);
GLenum (GLAPIENTRY* glGetError)(void);
void (GLAPIENTRY* glGetFloatv)(GLenum pname, GLfloat* data);
void (GLAPIENTRY* glGetIntegerv)(GLenum pname, GLint* data);
const GLubyte* (GLAPIENTRY* glGetString)(GLenum name);
void (GLAPIENTRY* glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
void (GLAPIENTRY* glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat* params);
void (GLAPIENTRY* glGetTexParameteriv)(GLenum target, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat* params);
void (GLAPIENTRY* glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint* params);
GLboolean (GLAPIENTRY* glIsEnabled)(GLenum cap);
void (GLAPIENTRY* glDepthRange)(GLdouble n, GLdouble f);
void (GLAPIENTRY* glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
#endif /* !GL_VERSION_1_0 */

/* ====================================
 * OpenGL 1.1
 */
#ifdef GL_VERSION_1_1
void (GLAPIENTRY* glDrawArrays)(GLenum mode, GLint first, GLsizei count);
void (GLAPIENTRY* glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
void (GLAPIENTRY* glGetPointerv)(GLenum pname, void** params);
void (GLAPIENTRY* glPolygonOffset)(GLfloat factor, GLfloat units);
void (GLAPIENTRY* glCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void (GLAPIENTRY* glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void (GLAPIENTRY* glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void (GLAPIENTRY* glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void (GLAPIENTRY* glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glBindTexture)(GLenum target, GLuint texture);
void (GLAPIENTRY* glDeleteTextures)(GLsizei n, const GLuint* textures);
void (GLAPIENTRY* glGenTextures)(GLsizei n, GLuint* textures);
GLboolean (GLAPIENTRY* glIsTexture)(GLuint texture);
#endif /* !GL_VERSION_1_1 */

/* ====================================
 * OpenGL 1.2
 */
#ifdef GL_VERSION_1_2
void (GLAPIENTRY* glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
void (GLAPIENTRY* glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#endif /* !GL_VERSION_1_2 */

/* ====================================
 * OpenGL 1.3
 */
#ifdef GL_VERSION_1_3
void (GLAPIENTRY* glActiveTexture)(GLenum texture);
void (GLAPIENTRY* glSampleCoverage)(GLfloat value, GLboolean invert);
void (GLAPIENTRY* glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glGetCompressedTexImage)(GLenum target, GLint level, void* img);
#endif /* !GL_VERSION_1_3 */

/* ====================================
 * OpenGL 1.4
 */
#ifdef GL_VERSION_1_4
void (GLAPIENTRY* glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void (GLAPIENTRY* glMultiDrawArrays)(GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount);
void (GLAPIENTRY* glMultiDrawElements)(GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount);
void (GLAPIENTRY* glPointParameterf)(GLenum pname, GLfloat param);
void (GLAPIENTRY* glPointParameterfv)(GLenum pname, const GLfloat* params);
void (GLAPIENTRY* glPointParameteri)(GLenum pname, GLint param);
void (GLAPIENTRY* glPointParameteriv)(GLenum pname, const GLint* params);
void (GLAPIENTRY* glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (GLAPIENTRY* glBlendEquation)(GLenum mode);
#endif /* !GL_VERSION_1_4 */

/* ====================================
 * OpenGL 1.5
 */
#ifdef GL_VERSION_1_5
void (GLAPIENTRY* glGenQueries)(GLsizei n, GLuint* ids);
void (GLAPIENTRY* glDeleteQueries)(GLsizei n, const GLuint* ids);
GLboolean (GLAPIENTRY* glIsQuery)(GLuint id);
void (GLAPIENTRY* glBeginQuery)(GLenum target, GLuint id);
void (GLAPIENTRY* glEndQuery)(GLenum target);
void (GLAPIENTRY* glGetQueryiv)(GLenum target, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetQueryObjectiv)(GLuint id, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint* params);
void (GLAPIENTRY* glBindBuffer)(GLenum target, GLuint buffer);
void (GLAPIENTRY* glDeleteBuffers)(GLsizei n, const GLuint* buffers);
void (GLAPIENTRY* glGenBuffers)(GLsizei n, GLuint* buffers);
GLboolean (GLAPIENTRY* glIsBuffer)(GLuint buffer);
void (GLAPIENTRY* glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
void (GLAPIENTRY* glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
void (GLAPIENTRY* glGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, void* data);
void* (GLAPIENTRY* glMapBuffer)(GLenum target, GLenum access);
GLboolean (GLAPIENTRY* glUnmapBuffer)(GLenum target);
void (GLAPIENTRY* glGetBufferParameteriv)(GLenum target, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetBufferPointerv)(GLenum target, GLenum pname, void** params);
#endif /* !GL_VERSION_1_5 */

/* ====================================
 * OpenGL 2.0
 */
#ifdef GL_VERSION_2_0
void (GLAPIENTRY* glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
void (GLAPIENTRY* glDrawBuffers)(GLsizei n, const GLenum* bufs);
void (GLAPIENTRY* glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void (GLAPIENTRY* glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
void (GLAPIENTRY* glStencilMaskSeparate)(GLenum face, GLuint mask);
void (GLAPIENTRY* glAttachShader)(GLuint program, GLuint shader);
void (GLAPIENTRY* glBindAttribLocation)(GLuint program, GLuint index, const GLchar* name);
void (GLAPIENTRY* glCompileShader)(GLuint shader);
GLuint (GLAPIENTRY* glCreateProgram)(void);
GLuint (GLAPIENTRY* glCreateShader)(GLenum type);
void (GLAPIENTRY* glDeleteProgram)(GLuint program);
void (GLAPIENTRY* glDeleteShader)(GLuint shader);
void (GLAPIENTRY* glDetachShader)(GLuint program, GLuint shader);
void (GLAPIENTRY* glDisableVertexAttribArray)(GLuint index);
void (GLAPIENTRY* glEnableVertexAttribArray)(GLuint index);
void (GLAPIENTRY* glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
void (GLAPIENTRY* glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
void (GLAPIENTRY* glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
GLint (GLAPIENTRY* glGetAttribLocation)(GLuint program, const GLchar* name);
void (GLAPIENTRY* glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void (GLAPIENTRY* glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void (GLAPIENTRY* glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source);
GLint (GLAPIENTRY* glGetUniformLocation)(GLuint program, const GLchar* name);
void (GLAPIENTRY* glGetUniformfv)(GLuint program, GLint location, GLfloat* params);
void (GLAPIENTRY* glGetUniformiv)(GLuint program, GLint location, GLint* params);
void (GLAPIENTRY* glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble* params);
void (GLAPIENTRY* glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat* params);
void (GLAPIENTRY* glGetVertexAttribiv)(GLuint index, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetVertexAttribPointerv)(GLuint index, GLenum pname, void** pointer);
GLboolean (GLAPIENTRY* glIsProgram)(GLuint program);
GLboolean (GLAPIENTRY* glIsShader)(GLuint shader);
void (GLAPIENTRY* glLinkProgram)(GLuint program);
void (GLAPIENTRY* glShaderSource)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
void (GLAPIENTRY* glUseProgram)(GLuint program);
void (GLAPIENTRY* glUniform1f)(GLint location, GLfloat v0);
void (GLAPIENTRY* glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
void (GLAPIENTRY* glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void (GLAPIENTRY* glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void (GLAPIENTRY* glUniform1i)(GLint location, GLint v0);
void (GLAPIENTRY* glUniform2i)(GLint location, GLint v0, GLint v1);
void (GLAPIENTRY* glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
void (GLAPIENTRY* glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void (GLAPIENTRY* glUniform1fv)(GLint location, GLsizei count, const GLfloat* value);
void (GLAPIENTRY* glUniform2fv)(GLint location, GLsizei count, const GLfloat* value);
void (GLAPIENTRY* glUniform3fv)(GLint location, GLsizei count, const GLfloat* value);
void (GLAPIENTRY* glUniform4fv)(GLint location, GLsizei count, const GLfloat* value);
void (GLAPIENTRY* glUniform1iv)(GLint location, GLsizei count, const GLint* value);
void (GLAPIENTRY* glUniform2iv)(GLint location, GLsizei count, const GLint* value);
void (GLAPIENTRY* glUniform3iv)(GLint location, GLsizei count, const GLint* value);
void (GLAPIENTRY* glUniform4iv)(GLint location, GLsizei count, const GLint* value);
void (GLAPIENTRY* glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glValidateProgram)(GLuint program);
void (GLAPIENTRY* glVertexAttrib1d)(GLuint index, GLdouble x);
void (GLAPIENTRY* glVertexAttrib1dv)(GLuint index, const GLdouble* v);
void (GLAPIENTRY* glVertexAttrib1f)(GLuint index, GLfloat x);
void (GLAPIENTRY* glVertexAttrib1fv)(GLuint index, const GLfloat* v);
void (GLAPIENTRY* glVertexAttrib1s)(GLuint index, GLshort x);
void (GLAPIENTRY* glVertexAttrib1sv)(GLuint index, const GLshort* v);
void (GLAPIENTRY* glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
void (GLAPIENTRY* glVertexAttrib2dv)(GLuint index, const GLdouble* v);
void (GLAPIENTRY* glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
void (GLAPIENTRY* glVertexAttrib2fv)(GLuint index, const GLfloat* v);
void (GLAPIENTRY* glVertexAttrib2s)(GLuint index, GLshort x, GLshort y);
void (GLAPIENTRY* glVertexAttrib2sv)(GLuint index, const GLshort* v);
void (GLAPIENTRY* glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void (GLAPIENTRY* glVertexAttrib3dv)(GLuint index, const GLdouble* v);
void (GLAPIENTRY* glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
void (GLAPIENTRY* glVertexAttrib3fv)(GLuint index, const GLfloat* v);
void (GLAPIENTRY* glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z);
void (GLAPIENTRY* glVertexAttrib3sv)(GLuint index, const GLshort* v);
void (GLAPIENTRY* glVertexAttrib4Nbv)(GLuint index, const GLbyte* v);
void (GLAPIENTRY* glVertexAttrib4Niv)(GLuint index, const GLint* v);
void (GLAPIENTRY* glVertexAttrib4Nsv)(GLuint index, const GLshort* v);
void (GLAPIENTRY* glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void (GLAPIENTRY* glVertexAttrib4Nubv)(GLuint index, const GLubyte* v);
void (GLAPIENTRY* glVertexAttrib4Nuiv)(GLuint index, const GLuint* v);
void (GLAPIENTRY* glVertexAttrib4Nusv)(GLuint index, const GLushort* v);
void (GLAPIENTRY* glVertexAttrib4bv)(GLuint index, const GLbyte* v);
void (GLAPIENTRY* glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (GLAPIENTRY* glVertexAttrib4dv)(GLuint index, const GLdouble* v);
void (GLAPIENTRY* glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (GLAPIENTRY* glVertexAttrib4fv)(GLuint index, const GLfloat* v);
void (GLAPIENTRY* glVertexAttrib4iv)(GLuint index, const GLint* v);
void (GLAPIENTRY* glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void (GLAPIENTRY* glVertexAttrib4sv)(GLuint index, const GLshort* v);
void (GLAPIENTRY* glVertexAttrib4ubv)(GLuint index, const GLubyte* v);
void (GLAPIENTRY* glVertexAttrib4uiv)(GLuint index, const GLuint* v);
void (GLAPIENTRY* glVertexAttrib4usv)(GLuint index, const GLushort* v);
void (GLAPIENTRY* glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
#endif /* !GL_VERSION_2_0 */

/* ====================================
 * OpenGL 2.1
 */
#ifdef GL_VERSION_2_1
void (GLAPIENTRY* glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
#endif /* !GL_VERSION_2_1 */

/* ====================================
 * OpenGL 3.0
 */
#ifdef GL_VERSION_3_0
void (GLAPIENTRY* glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void (GLAPIENTRY* glGetBooleani_v)(GLenum target, GLuint index, GLboolean* data);
void (GLAPIENTRY* glGetIntegeri_v)(GLenum target, GLuint index, GLint* data);
void (GLAPIENTRY* glEnablei)(GLenum target, GLuint index);
void (GLAPIENTRY* glDisablei)(GLenum target, GLuint index);
GLboolean (GLAPIENTRY* glIsEnabledi)(GLenum target, GLuint index);
void (GLAPIENTRY* glBeginTransformFeedback)(GLenum primitiveMode);
void (GLAPIENTRY* glEndTransformFeedback)(void);
void (GLAPIENTRY* glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void (GLAPIENTRY* glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
void (GLAPIENTRY* glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
void (GLAPIENTRY* glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
void (GLAPIENTRY* glClampColor)(GLenum target, GLenum clamp);
void (GLAPIENTRY* glBeginConditionalRender)(GLuint id, GLenum mode);
void (GLAPIENTRY* glEndConditionalRender)(void);
void (GLAPIENTRY* glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
void (GLAPIENTRY* glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint* params);
void (GLAPIENTRY* glVertexAttribI1i)(GLuint index, GLint x);
void (GLAPIENTRY* glVertexAttribI2i)(GLuint index, GLint x, GLint y);
void (GLAPIENTRY* glVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
void (GLAPIENTRY* glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
void (GLAPIENTRY* glVertexAttribI1ui)(GLuint index, GLuint x);
void (GLAPIENTRY* glVertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
void (GLAPIENTRY* glVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
void (GLAPIENTRY* glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void (GLAPIENTRY* glVertexAttribI1iv)(GLuint index, const GLint* v);
void (GLAPIENTRY* glVertexAttribI2iv)(GLuint index, const GLint* v);
void (GLAPIENTRY* glVertexAttribI3iv)(GLuint index, const GLint* v);
void (GLAPIENTRY* glVertexAttribI4iv)(GLuint index, const GLint* v);
void (GLAPIENTRY* glVertexAttribI1uiv)(GLuint index, const GLuint* v);
void (GLAPIENTRY* glVertexAttribI2uiv)(GLuint index, const GLuint* v);
void (GLAPIENTRY* glVertexAttribI3uiv)(GLuint index, const GLuint* v);
void (GLAPIENTRY* glVertexAttribI4uiv)(GLuint index, const GLuint* v);
void (GLAPIENTRY* glVertexAttribI4bv)(GLuint index, const GLbyte* v);
void (GLAPIENTRY* glVertexAttribI4sv)(GLuint index, const GLshort* v);
void (GLAPIENTRY* glVertexAttribI4ubv)(GLuint index, const GLubyte* v);
void (GLAPIENTRY* glVertexAttribI4usv)(GLuint index, const GLushort* v);
void (GLAPIENTRY* glGetUniformuiv)(GLuint program, GLint location, GLuint* params);
void (GLAPIENTRY* glBindFragDataLocation)(GLuint program, GLuint color, const GLchar* name);
GLint (GLAPIENTRY* glGetFragDataLocation)(GLuint program, const GLchar* name);
void (GLAPIENTRY* glUniform1ui)(GLint location, GLuint v0);
void (GLAPIENTRY* glUniform2ui)(GLint location, GLuint v0, GLuint v1);
void (GLAPIENTRY* glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
void (GLAPIENTRY* glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void (GLAPIENTRY* glUniform1uiv)(GLint location, GLsizei count, const GLuint* value);
void (GLAPIENTRY* glUniform2uiv)(GLint location, GLsizei count, const GLuint* value);
void (GLAPIENTRY* glUniform3uiv)(GLint location, GLsizei count, const GLuint* value);
void (GLAPIENTRY* glUniform4uiv)(GLint location, GLsizei count, const GLuint* value);
void (GLAPIENTRY* glTexParameterIiv)(GLenum target, GLenum pname, const GLint* params);
void (GLAPIENTRY* glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint* params);
void (GLAPIENTRY* glGetTexParameterIiv)(GLenum target, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint* params);
void (GLAPIENTRY* glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint* value);
void (GLAPIENTRY* glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint* value);
void (GLAPIENTRY* glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat* value);
void (GLAPIENTRY* glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
const GLubyte* (GLAPIENTRY* glGetStringi)(GLenum name, GLuint index);
GLboolean (GLAPIENTRY* glIsRenderbuffer)(GLuint renderbuffer);
void (GLAPIENTRY* glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
void (GLAPIENTRY* glDeleteRenderbuffers)(GLsizei n, const GLuint* renderbuffers);
void (GLAPIENTRY* glGenRenderbuffers)(GLsizei n, GLuint* renderbuffers);
void (GLAPIENTRY* glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void (GLAPIENTRY* glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint* params);
GLboolean (GLAPIENTRY* glIsFramebuffer)(GLuint framebuffer);
void (GLAPIENTRY* glBindFramebuffer)(GLenum target, GLuint framebuffer);
void (GLAPIENTRY* glDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
void (GLAPIENTRY* glGenFramebuffers)(GLsizei n, GLuint* framebuffers);
GLenum (GLAPIENTRY* glCheckFramebufferStatus)(GLenum target);
void (GLAPIENTRY* glFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void (GLAPIENTRY* glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void (GLAPIENTRY* glFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void (GLAPIENTRY* glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void (GLAPIENTRY* glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint* params);
void (GLAPIENTRY* glGenerateMipmap)(GLenum target);
void (GLAPIENTRY* glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
void (GLAPIENTRY* glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void (GLAPIENTRY* glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
void* (GLAPIENTRY* glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void (GLAPIENTRY* glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
void (GLAPIENTRY* glBindVertexArray)(GLuint array);
void (GLAPIENTRY* glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
void (GLAPIENTRY* glGenVertexArrays)(GLsizei n, GLuint* arrays);
GLboolean (GLAPIENTRY* glIsVertexArray)(GLuint array);
#endif /* !GL_VERSION_3_0 */

/* ====================================
 * OpenGL 3.1
 */
#ifdef GL_VERSION_3_1
void (GLAPIENTRY* glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
void (GLAPIENTRY* glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
void (GLAPIENTRY* glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
void (GLAPIENTRY* glPrimitiveRestartIndex)(GLuint index);
void (GLAPIENTRY* glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void (GLAPIENTRY* glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices);
void (GLAPIENTRY* glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
GLuint (GLAPIENTRY* glGetUniformBlockIndex)(GLuint program, const GLchar* uniformBlockName);
void (GLAPIENTRY* glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
void (GLAPIENTRY* glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
#endif /* !GL_VERSION_3_1 */

/* ====================================
 * OpenGL 3.2
 */
#ifdef GL_VERSION_3_2
void (GLAPIENTRY* glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex);
void (GLAPIENTRY* glDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices, GLint basevertex);
void (GLAPIENTRY* glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex);
void (GLAPIENTRY* glMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount, const GLint* basevertex);
void (GLAPIENTRY* glProvokingVertex)(GLenum mode);
GLsync (GLAPIENTRY* glFenceSync)(GLenum condition, GLbitfield flags);
GLboolean (GLAPIENTRY* glIsSync)(GLsync sync);
void (GLAPIENTRY* glDeleteSync)(GLsync sync);
GLenum (GLAPIENTRY* glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
void (GLAPIENTRY* glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
void (GLAPIENTRY* glGetInteger64v)(GLenum pname, GLint64* data);
void (GLAPIENTRY* glGetSynciv)(GLsync sync, GLenum pname, GLsizei count, GLsizei* length, GLint* values);
void (GLAPIENTRY* glGetInteger64i_v)(GLenum target, GLuint index, GLint64* data);
void (GLAPIENTRY* glGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64* params);
void (GLAPIENTRY* glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
void (GLAPIENTRY* glTexImage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void (GLAPIENTRY* glTexImage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void (GLAPIENTRY* glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat* val);
void (GLAPIENTRY* glSampleMaski)(GLuint maskNumber, GLbitfield mask);
#endif /* !GL_VERSION_3_2 */

/* ====================================
 * OpenGL 3.3
 */
#ifdef GL_VERSION_3_3
void (GLAPIENTRY* glBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar* name);
GLint (GLAPIENTRY* glGetFragDataIndex)(GLuint program, const GLchar* name);
void (GLAPIENTRY* glGenSamplers)(GLsizei count, GLuint* samplers);
void (GLAPIENTRY* glDeleteSamplers)(GLsizei count, const GLuint* samplers);
GLboolean (GLAPIENTRY* glIsSampler)(GLuint sampler);
void (GLAPIENTRY* glBindSampler)(GLuint unit, GLuint sampler);
void (GLAPIENTRY* glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
void (GLAPIENTRY* glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint* param);
void (GLAPIENTRY* glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
void (GLAPIENTRY* glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat* param);
void (GLAPIENTRY* glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint* param);
void (GLAPIENTRY* glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint* param);
void (GLAPIENTRY* glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat* params);
void (GLAPIENTRY* glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint* params);
void (GLAPIENTRY* glQueryCounter)(GLuint id, GLenum target);
void (GLAPIENTRY* glGetQueryObjecti64v)(GLuint id, GLenum pname, GLint64* params);
void (GLAPIENTRY* glGetQueryObjectui64v)(GLuint id, GLenum pname, GLuint64* params);
void (GLAPIENTRY* glVertexAttribDivisor)(GLuint index, GLuint divisor);
void (GLAPIENTRY* glVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void (GLAPIENTRY* glVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void (GLAPIENTRY* glVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void (GLAPIENTRY* glVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void (GLAPIENTRY* glVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void (GLAPIENTRY* glVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void (GLAPIENTRY* glVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void (GLAPIENTRY* glVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
#endif /* !GL_VERSION_3_3 */

/* ====================================
 * OpenGL 4.0
 */
#ifdef GL_VERSION_4_0
void (GLAPIENTRY* glMinSampleShading)(GLfloat value);
void (GLAPIENTRY* glBlendEquationi)(GLuint buf, GLenum mode);
void (GLAPIENTRY* glBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
void (GLAPIENTRY* glBlendFunci)(GLuint buf, GLenum src, GLenum dst);
void (GLAPIENTRY* glBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void (GLAPIENTRY* glDrawArraysIndirect)(GLenum mode, const void* indirect);
void (GLAPIENTRY* glDrawElementsIndirect)(GLenum mode, GLenum type, const void* indirect);
void (GLAPIENTRY* glUniform1d)(GLint location, GLdouble x);
void (GLAPIENTRY* glUniform2d)(GLint location, GLdouble x, GLdouble y);
void (GLAPIENTRY* glUniform3d)(GLint location, GLdouble x, GLdouble y, GLdouble z);
void (GLAPIENTRY* glUniform4d)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (GLAPIENTRY* glUniform1dv)(GLint location, GLsizei count, const GLdouble* value);
void (GLAPIENTRY* glUniform2dv)(GLint location, GLsizei count, const GLdouble* value);
void (GLAPIENTRY* glUniform3dv)(GLint location, GLsizei count, const GLdouble* value);
void (GLAPIENTRY* glUniform4dv)(GLint location, GLsizei count, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix2x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix2x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix3x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix3x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix4x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glUniformMatrix4x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glGetUniformdv)(GLuint program, GLint location, GLdouble* params);
GLint (GLAPIENTRY* glGetSubroutineUniformLocation)(GLuint program, GLenum shadertype, const GLchar* name);
GLuint (GLAPIENTRY* glGetSubroutineIndex)(GLuint program, GLenum shadertype, const GLchar* name);
void (GLAPIENTRY* glGetActiveSubroutineUniformiv)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
void (GLAPIENTRY* glGetActiveSubroutineUniformName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei* length, GLchar* name);
void (GLAPIENTRY* glGetActiveSubroutineName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei* length, GLchar* name);
void (GLAPIENTRY* glUniformSubroutinesuiv)(GLenum shadertype, GLsizei count, const GLuint* indices);
void (GLAPIENTRY* glGetUniformSubroutineuiv)(GLenum shadertype, GLint location, GLuint* params);
void (GLAPIENTRY* glGetProgramStageiv)(GLuint program, GLenum shadertype, GLenum pname, GLint* values);
void (GLAPIENTRY* glPatchParameteri)(GLenum pname, GLint value);
void (GLAPIENTRY* glPatchParameterfv)(GLenum pname, const GLfloat* values);
void (GLAPIENTRY* glBindTransformFeedback)(GLenum target, GLuint id);
void (GLAPIENTRY* glDeleteTransformFeedbacks)(GLsizei n, const GLuint* ids);
void (GLAPIENTRY* glGenTransformFeedbacks)(GLsizei n, GLuint* ids);
GLboolean (GLAPIENTRY* glIsTransformFeedback)(GLuint id);
void (GLAPIENTRY* glPauseTransformFeedback)(void);
void (GLAPIENTRY* glResumeTransformFeedback)(void);
void (GLAPIENTRY* glDrawTransformFeedback)(GLenum mode, GLuint id);
void (GLAPIENTRY* glDrawTransformFeedbackStream)(GLenum mode, GLuint id, GLuint stream);
void (GLAPIENTRY* glBeginQueryIndexed)(GLenum target, GLuint index, GLuint id);
void (GLAPIENTRY* glEndQueryIndexed)(GLenum target, GLuint index);
void (GLAPIENTRY* glGetQueryIndexediv)(GLenum target, GLuint index, GLenum pname, GLint* params);
#endif /* !GL_VERSION_4_0 */

/* ====================================
 * OpenGL 4.1
 */
#ifdef GL_VERSION_4_1
void (GLAPIENTRY* glReleaseShaderCompiler)(void);
void (GLAPIENTRY* glShaderBinary)(GLsizei count, const GLuint* shaders, GLenum binaryFormat, const void* binary, GLsizei length);
void (GLAPIENTRY* glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
void (GLAPIENTRY* glDepthRangef)(GLfloat n, GLfloat f);
void (GLAPIENTRY* glClearDepthf)(GLfloat d);
void (GLAPIENTRY* glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void* binary);
void (GLAPIENTRY* glProgramBinary)(GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
void (GLAPIENTRY* glProgramParameteri)(GLuint program, GLenum pname, GLint value);
void (GLAPIENTRY* glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
void (GLAPIENTRY* glActiveShaderProgram)(GLuint pipeline, GLuint program);
GLuint (GLAPIENTRY* glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar* const* strings);
void (GLAPIENTRY* glBindProgramPipeline)(GLuint pipeline);
void (GLAPIENTRY* glDeleteProgramPipelines)(GLsizei n, const GLuint* pipelines);
void (GLAPIENTRY* glGenProgramPipelines)(GLsizei n, GLuint* pipelines);
GLboolean (GLAPIENTRY* glIsProgramPipeline)(GLuint pipeline);
void (GLAPIENTRY* glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint* params);
void (GLAPIENTRY* glProgramUniform1i)(GLuint program, GLint location, GLint v0);
void (GLAPIENTRY* glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
void (GLAPIENTRY* glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
void (GLAPIENTRY* glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
void (GLAPIENTRY* glProgramUniform1d)(GLuint program, GLint location, GLdouble v0);
void (GLAPIENTRY* glProgramUniform1dv)(GLuint program, GLint location, GLsizei count, const GLdouble* value);
void (GLAPIENTRY* glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
void (GLAPIENTRY* glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
void (GLAPIENTRY* glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
void (GLAPIENTRY* glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
void (GLAPIENTRY* glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
void (GLAPIENTRY* glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
void (GLAPIENTRY* glProgramUniform2d)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
void (GLAPIENTRY* glProgramUniform2dv)(GLuint program, GLint location, GLsizei count, const GLdouble* value);
void (GLAPIENTRY* glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
void (GLAPIENTRY* glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
void (GLAPIENTRY* glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
void (GLAPIENTRY* glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
void (GLAPIENTRY* glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void (GLAPIENTRY* glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
void (GLAPIENTRY* glProgramUniform3d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
void (GLAPIENTRY* glProgramUniform3dv)(GLuint program, GLint location, GLsizei count, const GLdouble* value);
void (GLAPIENTRY* glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
void (GLAPIENTRY* glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
void (GLAPIENTRY* glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void (GLAPIENTRY* glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
void (GLAPIENTRY* glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void (GLAPIENTRY* glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
void (GLAPIENTRY* glProgramUniform4d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
void (GLAPIENTRY* glProgramUniform4dv)(GLuint program, GLint location, GLsizei count, const GLdouble* value);
void (GLAPIENTRY* glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void (GLAPIENTRY* glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
void (GLAPIENTRY* glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glProgramUniformMatrix3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glProgramUniformMatrix4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void (GLAPIENTRY* glProgramUniformMatrix2x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glProgramUniformMatrix3x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glProgramUniformMatrix2x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glProgramUniformMatrix4x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glProgramUniformMatrix3x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glProgramUniformMatrix4x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void (GLAPIENTRY* glValidateProgramPipeline)(GLuint pipeline);
void (GLAPIENTRY* glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void (GLAPIENTRY* glVertexAttribL1d)(GLuint index, GLdouble x);
void (GLAPIENTRY* glVertexAttribL2d)(GLuint index, GLdouble x, GLdouble y);
void (GLAPIENTRY* glVertexAttribL3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void (GLAPIENTRY* glVertexAttribL4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (GLAPIENTRY* glVertexAttribL1dv)(GLuint index, const GLdouble* v);
void (GLAPIENTRY* glVertexAttribL2dv)(GLuint index, const GLdouble* v);
void (GLAPIENTRY* glVertexAttribL3dv)(GLuint index, const GLdouble* v);
void (GLAPIENTRY* glVertexAttribL4dv)(GLuint index, const GLdouble* v);
void (GLAPIENTRY* glVertexAttribLPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
void (GLAPIENTRY* glGetVertexAttribLdv)(GLuint index, GLenum pname, GLdouble* params);
void (GLAPIENTRY* glViewportArrayv)(GLuint first, GLsizei count, const GLfloat* v);
void (GLAPIENTRY* glViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
void (GLAPIENTRY* glViewportIndexedfv)(GLuint index, const GLfloat* v);
void (GLAPIENTRY* glScissorArrayv)(GLuint first, GLsizei count, const GLint* v);
void (GLAPIENTRY* glScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
void (GLAPIENTRY* glScissorIndexedv)(GLuint index, const GLint* v);
void (GLAPIENTRY* glDepthRangeArrayv)(GLuint first, GLsizei count, const GLdouble* v);
void (GLAPIENTRY* glDepthRangeIndexed)(GLuint index, GLdouble n, GLdouble f);
void (GLAPIENTRY* glGetFloati_v)(GLenum target, GLuint index, GLfloat* data);
void (GLAPIENTRY* glGetDoublei_v)(GLenum target, GLuint index, GLdouble* data);
#endif /* !GL_VERSION_4_1 */

/* ====================================
 * OpenGL 4.2
 */
#ifdef GL_VERSION_4_2
void (GLAPIENTRY* glDrawArraysInstancedBaseInstance)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
void (GLAPIENTRY* glDrawElementsInstancedBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLuint baseinstance);
void (GLAPIENTRY* glDrawElementsInstancedBaseVertexBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
void (GLAPIENTRY* glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint* params);
void (GLAPIENTRY* glGetActiveAtomicCounterBufferiv)(GLuint program, GLuint bufferIndex, GLenum pname, GLint* params);
void (GLAPIENTRY* glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
void (GLAPIENTRY* glMemoryBarrier)(GLbitfield barriers);
void (GLAPIENTRY* glTexStorage1D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
void (GLAPIENTRY* glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
void (GLAPIENTRY* glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
void (GLAPIENTRY* glDrawTransformFeedbackInstanced)(GLenum mode, GLuint id, GLsizei instancecount);
void (GLAPIENTRY* glDrawTransformFeedbackStreamInstanced)(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
#endif /* !GL_VERSION_4_2 */

/* ====================================
 * OpenGL 4.3
 */
#ifdef GL_VERSION_4_3
void (GLAPIENTRY* glClearBufferData)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void* data);
void (GLAPIENTRY* glClearBufferSubData)(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void* data);
void (GLAPIENTRY* glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
void (GLAPIENTRY* glDispatchComputeIndirect)(GLintptr indirect);
void (GLAPIENTRY* glCopyImageSubData)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
void (GLAPIENTRY* glFramebufferParameteri)(GLenum target, GLenum pname, GLint param);
void (GLAPIENTRY* glGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetInternalformati64v)(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint64* params);
void (GLAPIENTRY* glInvalidateTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
void (GLAPIENTRY* glInvalidateTexImage)(GLuint texture, GLint level);
void (GLAPIENTRY* glInvalidateBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr length);
void (GLAPIENTRY* glInvalidateBufferData)(GLuint buffer);
void (GLAPIENTRY* glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum* attachments);
void (GLAPIENTRY* glInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
void (GLAPIENTRY* glMultiDrawArraysIndirect)(GLenum mode, const void* indirect, GLsizei drawcount, GLsizei stride);
void (GLAPIENTRY* glMultiDrawElementsIndirect)(GLenum mode, GLenum type, const void* indirect, GLsizei drawcount, GLsizei stride);
void (GLAPIENTRY* glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint* params);
GLuint (GLAPIENTRY* glGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar* name);
void (GLAPIENTRY* glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, GLchar* name);
void (GLAPIENTRY* glGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei count, GLsizei* length, GLint* params);
GLint (GLAPIENTRY* glGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar* name);
GLint (GLAPIENTRY* glGetProgramResourceLocationIndex)(GLuint program, GLenum programInterface, const GLchar* name);
void (GLAPIENTRY* glShaderStorageBlockBinding)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
void (GLAPIENTRY* glTexBufferRange)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void (GLAPIENTRY* glTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void (GLAPIENTRY* glTexStorage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void (GLAPIENTRY* glTextureView)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
void (GLAPIENTRY* glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void (GLAPIENTRY* glVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
void (GLAPIENTRY* glVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void (GLAPIENTRY* glVertexAttribLFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void (GLAPIENTRY* glVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);
void (GLAPIENTRY* glVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);
void (GLAPIENTRY* glDebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
void (GLAPIENTRY* glDebugMessageInsert)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
void (GLAPIENTRY* glDebugMessageCallback)(GLDEBUGPROC callback, const void* userParam);
GLuint (GLAPIENTRY* glGetDebugMessageLog)(GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);
void (GLAPIENTRY* glPushDebugGroup)(GLenum source, GLuint id, GLsizei length, const GLchar* message);
void (GLAPIENTRY* glPopDebugGroup)(void);
void (GLAPIENTRY* glObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar* label);
void (GLAPIENTRY* glGetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar* label);
void (GLAPIENTRY* glObjectPtrLabel)(const void* ptr, GLsizei length, const GLchar* label);
void (GLAPIENTRY* glGetObjectPtrLabel)(const void* ptr, GLsizei bufSize, GLsizei* length, GLchar* label);
#endif /* !GL_VERSION_4_3 */

/* ====================================
 * OpenGL 4.4
 */
#ifdef GL_VERSION_4_4
void (GLAPIENTRY* glBufferStorage)(GLenum target, GLsizeiptr size, const void* data, GLbitfield flags);
void (GLAPIENTRY* glClearTexImage)(GLuint texture, GLint level, GLenum format, GLenum type, const void* data);
void (GLAPIENTRY* glClearTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* data);
void (GLAPIENTRY* glBindBuffersBase)(GLenum target, GLuint first, GLsizei count, const GLuint* buffers);
void (GLAPIENTRY* glBindBuffersRange)(GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizeiptr* sizes);
void (GLAPIENTRY* glBindTextures)(GLuint first, GLsizei count, const GLuint* textures);
void (GLAPIENTRY* glBindSamplers)(GLuint first, GLsizei count, const GLuint* samplers);
void (GLAPIENTRY* glBindImageTextures)(GLuint first, GLsizei count, const GLuint* textures);
void (GLAPIENTRY* glBindVertexBuffers)(GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizei* strides);
#endif /* !GL_VERSION_4_4 */

/* ====================================
 * OpenGL 4.5
 */
#ifdef GL_VERSION_4_5
void (GLAPIENTRY* glClipControl)(GLenum origin, GLenum depth);
void (GLAPIENTRY* glCreateTransformFeedbacks)(GLsizei n, GLuint* ids);
void (GLAPIENTRY* glTransformFeedbackBufferBase)(GLuint xfb, GLuint index, GLuint buffer);
void (GLAPIENTRY* glTransformFeedbackBufferRange)(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void (GLAPIENTRY* glGetTransformFeedbackiv)(GLuint xfb, GLenum pname, GLint* param);
void (GLAPIENTRY* glGetTransformFeedbacki_v)(GLuint xfb, GLenum pname, GLuint index, GLint* param);
void (GLAPIENTRY* glGetTransformFeedbacki64_v)(GLuint xfb, GLenum pname, GLuint index, GLint64* param);
void (GLAPIENTRY* glCreateBuffers)(GLsizei n, GLuint* buffers);
void (GLAPIENTRY* glNamedBufferStorage)(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags);
void (GLAPIENTRY* glNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
void (GLAPIENTRY* glNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
void (GLAPIENTRY* glCopyNamedBufferSubData)(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void (GLAPIENTRY* glClearNamedBufferData)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void* data);
void (GLAPIENTRY* glClearNamedBufferSubData)(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void* data);
void* (GLAPIENTRY* glMapNamedBuffer)(GLuint buffer, GLenum access);
void* (GLAPIENTRY* glMapNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLboolean (GLAPIENTRY* glUnmapNamedBuffer)(GLuint buffer);
void (GLAPIENTRY* glFlushMappedNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length);
void (GLAPIENTRY* glGetNamedBufferParameteriv)(GLuint buffer, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetNamedBufferParameteri64v)(GLuint buffer, GLenum pname, GLint64* params);
void (GLAPIENTRY* glGetNamedBufferPointerv)(GLuint buffer, GLenum pname, void** params);
void (GLAPIENTRY* glGetNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
void (GLAPIENTRY* glCreateFramebuffers)(GLsizei n, GLuint* framebuffers);
void (GLAPIENTRY* glNamedFramebufferRenderbuffer)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void (GLAPIENTRY* glNamedFramebufferParameteri)(GLuint framebuffer, GLenum pname, GLint param);
void (GLAPIENTRY* glNamedFramebufferTexture)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
void (GLAPIENTRY* glNamedFramebufferTextureLayer)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
void (GLAPIENTRY* glNamedFramebufferDrawBuffer)(GLuint framebuffer, GLenum buf);
void (GLAPIENTRY* glNamedFramebufferDrawBuffers)(GLuint framebuffer, GLsizei n, const GLenum* bufs);
void (GLAPIENTRY* glNamedFramebufferReadBuffer)(GLuint framebuffer, GLenum src);
void (GLAPIENTRY* glInvalidateNamedFramebufferData)(GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments);
void (GLAPIENTRY* glInvalidateNamedFramebufferSubData)(GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
void (GLAPIENTRY* glClearNamedFramebufferiv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value);
void (GLAPIENTRY* glClearNamedFramebufferuiv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value);
void (GLAPIENTRY* glClearNamedFramebufferfv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value);
void (GLAPIENTRY* glClearNamedFramebufferfi)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
void (GLAPIENTRY* glBlitNamedFramebuffer)(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLenum (GLAPIENTRY* glCheckNamedFramebufferStatus)(GLuint framebuffer, GLenum target);
void (GLAPIENTRY* glGetNamedFramebufferParameteriv)(GLuint framebuffer, GLenum pname, GLint* param);
void (GLAPIENTRY* glGetNamedFramebufferAttachmentParameteriv)(GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
void (GLAPIENTRY* glCreateRenderbuffers)(GLsizei n, GLuint* renderbuffers);
void (GLAPIENTRY* glNamedRenderbufferStorage)(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
void (GLAPIENTRY* glNamedRenderbufferStorageMultisample)(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void (GLAPIENTRY* glGetNamedRenderbufferParameteriv)(GLuint renderbuffer, GLenum pname, GLint* params);
void (GLAPIENTRY* glCreateTextures)(GLenum target, GLsizei n, GLuint* textures);
void (GLAPIENTRY* glTextureBuffer)(GLuint texture, GLenum internalformat, GLuint buffer);
void (GLAPIENTRY* glTextureBufferRange)(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void (GLAPIENTRY* glTextureStorage1D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
void (GLAPIENTRY* glTextureStorage2D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
void (GLAPIENTRY* glTextureStorage3D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
void (GLAPIENTRY* glTextureStorage2DMultisample)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void (GLAPIENTRY* glTextureStorage3DMultisample)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void (GLAPIENTRY* glTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
void (GLAPIENTRY* glCompressedTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glCompressedTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glCompressedTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
void (GLAPIENTRY* glCopyTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void (GLAPIENTRY* glCopyTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void (GLAPIENTRY* glCopyTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void (GLAPIENTRY* glTextureParameterf)(GLuint texture, GLenum pname, GLfloat param);
void (GLAPIENTRY* glTextureParameterfv)(GLuint texture, GLenum pname, const GLfloat* param);
void (GLAPIENTRY* glTextureParameteri)(GLuint texture, GLenum pname, GLint param);
void (GLAPIENTRY* glTextureParameterIiv)(GLuint texture, GLenum pname, const GLint* params);
void (GLAPIENTRY* glTextureParameterIuiv)(GLuint texture, GLenum pname, const GLuint* params);
void (GLAPIENTRY* glTextureParameteriv)(GLuint texture, GLenum pname, const GLint* param);
void (GLAPIENTRY* glGenerateTextureMipmap)(GLuint texture);
void (GLAPIENTRY* glBindTextureUnit)(GLuint unit, GLuint texture);
void (GLAPIENTRY* glGetTextureImage)(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
void (GLAPIENTRY* glGetCompressedTextureImage)(GLuint texture, GLint level, GLsizei bufSize, void* pixels);
void (GLAPIENTRY* glGetTextureLevelParameterfv)(GLuint texture, GLint level, GLenum pname, GLfloat* params);
void (GLAPIENTRY* glGetTextureLevelParameteriv)(GLuint texture, GLint level, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetTextureParameterfv)(GLuint texture, GLenum pname, GLfloat* params);
void (GLAPIENTRY* glGetTextureParameterIiv)(GLuint texture, GLenum pname, GLint* params);
void (GLAPIENTRY* glGetTextureParameterIuiv)(GLuint texture, GLenum pname, GLuint* params);
void (GLAPIENTRY* glGetTextureParameteriv)(GLuint texture, GLenum pname, GLint* params);
void (GLAPIENTRY* glCreateVertexArrays)(GLsizei n, GLuint* arrays);
void (GLAPIENTRY* glDisableVertexArrayAttrib)(GLuint vaobj, GLuint index);
void (GLAPIENTRY* glEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
void (GLAPIENTRY* glVertexArrayElementBuffer)(GLuint vaobj, GLuint buffer);
void (GLAPIENTRY* glVertexArrayVertexBuffer)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void (GLAPIENTRY* glVertexArrayVertexBuffers)(GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizei* strides);
void (GLAPIENTRY* glVertexArrayAttribBinding)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
void (GLAPIENTRY* glVertexArrayAttribFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
void (GLAPIENTRY* glVertexArrayAttribIFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void (GLAPIENTRY* glVertexArrayAttribLFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void (GLAPIENTRY* glVertexArrayBindingDivisor)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
void (GLAPIENTRY* glGetVertexArrayiv)(GLuint vaobj, GLenum pname, GLint* param);
void (GLAPIENTRY* glGetVertexArrayIndexediv)(GLuint vaobj, GLuint index, GLenum pname, GLint* param);
void (GLAPIENTRY* glGetVertexArrayIndexed64iv)(GLuint vaobj, GLuint index, GLenum pname, GLint64* param);
void (GLAPIENTRY* glCreateSamplers)(GLsizei n, GLuint* samplers);
void (GLAPIENTRY* glCreateProgramPipelines)(GLsizei n, GLuint* pipelines);
void (GLAPIENTRY* glCreateQueries)(GLenum target, GLsizei n, GLuint* ids);
void (GLAPIENTRY* glGetQueryBufferObjecti64v)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void (GLAPIENTRY* glGetQueryBufferObjectiv)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void (GLAPIENTRY* glGetQueryBufferObjectui64v)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void (GLAPIENTRY* glGetQueryBufferObjectuiv)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void (GLAPIENTRY* glMemoryBarrierByRegion)(GLbitfield barriers);
void (GLAPIENTRY* glGetTextureSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
void (GLAPIENTRY* glGetCompressedTextureSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void* pixels);
GLenum (GLAPIENTRY* glGetGraphicsResetStatus)(void);
void (GLAPIENTRY* glGetnCompressedTexImage)(GLenum target, GLint lod, GLsizei bufSize, void* pixels);
void (GLAPIENTRY* glGetnTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
void (GLAPIENTRY* glGetnUniformdv)(GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
void (GLAPIENTRY* glGetnUniformfv)(GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
void (GLAPIENTRY* glGetnUniformiv)(GLuint program, GLint location, GLsizei bufSize, GLint* params);
void (GLAPIENTRY* glGetnUniformuiv)(GLuint program, GLint location, GLsizei bufSize, GLuint* params);
void (GLAPIENTRY* glReadnPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);
void (GLAPIENTRY* glTextureBarrier)(void);
#endif /* !GL_VERSION_4_5 */

/* ====================================
 * OpenGL 4.6
 */
#ifdef GL_VERSION_4_6
void (GLAPIENTRY* glSpecializeShader)(GLuint shader, const GLchar* pEntryPoint, GLuint numSpecializationConstants, const GLuint* pConstantIndex, const GLuint* pConstantValue);
void (GLAPIENTRY* glMultiDrawArraysIndirectCount)(GLenum mode, const void* indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
void (GLAPIENTRY* glMultiDrawElementsIndirectCount)(GLenum mode, GLenum type, const void* indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
void (GLAPIENTRY* glPolygonOffsetClamp)(GLfloat factor, GLfloat units, GLfloat clamp);
#endif /* !GL_VERSION_4_6 */


/* ====================================
 * InitOpenGL_1_0
 */
bool InitOpenGL_1_0() {
	bool load_result = false;
#ifdef GL_VERSION_1_0
	cb_GetProcAddress(glCullFace);
	cb_GetProcAddress(glFrontFace);
	cb_GetProcAddress(glHint);
	cb_GetProcAddress(glLineWidth);
	cb_GetProcAddress(glPointSize);
	cb_GetProcAddress(glPolygonMode);
	cb_GetProcAddress(glScissor);
	cb_GetProcAddress(glTexParameterf);
	cb_GetProcAddress(glTexParameterfv);
	cb_GetProcAddress(glTexParameteri);
	cb_GetProcAddress(glTexParameteriv);
	cb_GetProcAddress(glTexImage1D);
	cb_GetProcAddress(glTexImage2D);
	cb_GetProcAddress(glDrawBuffer);
	cb_GetProcAddress(glClear);
	cb_GetProcAddress(glClearColor);
	cb_GetProcAddress(glClearStencil);
	cb_GetProcAddress(glClearDepth);
	cb_GetProcAddress(glStencilMask);
	cb_GetProcAddress(glColorMask);
	cb_GetProcAddress(glDepthMask);
	cb_GetProcAddress(glDisable);
	cb_GetProcAddress(glEnable);
	cb_GetProcAddress(glFinish);
	cb_GetProcAddress(glFlush);
	cb_GetProcAddress(glBlendFunc);
	cb_GetProcAddress(glLogicOp);
	cb_GetProcAddress(glStencilFunc);
	cb_GetProcAddress(glStencilOp);
	cb_GetProcAddress(glDepthFunc);
	cb_GetProcAddress(glPixelStoref);
	cb_GetProcAddress(glPixelStorei);
	cb_GetProcAddress(glReadBuffer);
	cb_GetProcAddress(glReadPixels);
	cb_GetProcAddress(glGetBooleanv);
	cb_GetProcAddress(glGetDoublev);
	cb_GetProcAddress(glGetError);
	cb_GetProcAddress(glGetFloatv);
	cb_GetProcAddress(glGetIntegerv);
	cb_GetProcAddress(glGetString);
	cb_GetProcAddress(glGetTexImage);
	cb_GetProcAddress(glGetTexParameterfv);
	cb_GetProcAddress(glGetTexParameteriv);
	cb_GetProcAddress(glGetTexLevelParameterfv);
	cb_GetProcAddress(glGetTexLevelParameteriv);
	cb_GetProcAddress(glIsEnabled);
	cb_GetProcAddress(glDepthRange);
	cb_GetProcAddress(glViewport);
#endif /* !GL_VERSION_1_0 */
	return load_result;
}

/* ====================================
 * InitOpenGL_1_1
 */
bool InitOpenGL_1_1() {
	bool load_result = false;
#ifdef GL_VERSION_1_1
	cb_GetProcAddress(glDrawArrays);
	cb_GetProcAddress(glDrawElements);
	cb_GetProcAddress(glGetPointerv);
	cb_GetProcAddress(glPolygonOffset);
	cb_GetProcAddress(glCopyTexImage1D);
	cb_GetProcAddress(glCopyTexImage2D);
	cb_GetProcAddress(glCopyTexSubImage1D);
	cb_GetProcAddress(glCopyTexSubImage2D);
	cb_GetProcAddress(glTexSubImage1D);
	cb_GetProcAddress(glTexSubImage2D);
	cb_GetProcAddress(glBindTexture);
	cb_GetProcAddress(glDeleteTextures);
	cb_GetProcAddress(glGenTextures);
	cb_GetProcAddress(glIsTexture);
#endif /* !GL_VERSION_1_1 */
	return load_result;
}

/* ====================================
 * InitOpenGL_1_2
 */
bool InitOpenGL_1_2() {
	bool load_result = false;
#ifdef GL_VERSION_1_2
	cb_GetProcAddress(glDrawRangeElements);
	cb_GetProcAddress(glTexImage3D);
	cb_GetProcAddress(glTexSubImage3D);
	cb_GetProcAddress(glCopyTexSubImage3D);
#endif /* !GL_VERSION_1_2 */
	return load_result;
}

/* ====================================
 * InitOpenGL_1_3
 */
bool InitOpenGL_1_3() {
	bool load_result = false;
#ifdef GL_VERSION_1_3
	cb_GetProcAddress(glActiveTexture);
	cb_GetProcAddress(glSampleCoverage);
	cb_GetProcAddress(glCompressedTexImage3D);
	cb_GetProcAddress(glCompressedTexImage2D);
	cb_GetProcAddress(glCompressedTexImage1D);
	cb_GetProcAddress(glCompressedTexSubImage3D);
	cb_GetProcAddress(glCompressedTexSubImage2D);
	cb_GetProcAddress(glCompressedTexSubImage1D);
	cb_GetProcAddress(glGetCompressedTexImage);
#endif /* !GL_VERSION_1_3 */
	return load_result;
}

/* ====================================
 * InitOpenGL_1_4
 */
bool InitOpenGL_1_4() {
	bool load_result = false;
#ifdef GL_VERSION_1_4
	cb_GetProcAddress(glBlendFuncSeparate);
	cb_GetProcAddress(glMultiDrawArrays);
	cb_GetProcAddress(glMultiDrawElements);
	cb_GetProcAddress(glPointParameterf);
	cb_GetProcAddress(glPointParameterfv);
	cb_GetProcAddress(glPointParameteri);
	cb_GetProcAddress(glPointParameteriv);
	cb_GetProcAddress(glBlendColor);
	cb_GetProcAddress(glBlendEquation);
#endif /* !GL_VERSION_1_4 */
	return load_result;
}

/* ====================================
 * InitOpenGL_1_5
 */
bool InitOpenGL_1_5() {
	bool load_result = false;
#ifdef GL_VERSION_1_5
	cb_GetProcAddress(glGenQueries);
	cb_GetProcAddress(glDeleteQueries);
	cb_GetProcAddress(glIsQuery);
	cb_GetProcAddress(glBeginQuery);
	cb_GetProcAddress(glEndQuery);
	cb_GetProcAddress(glGetQueryiv);
	cb_GetProcAddress(glGetQueryObjectiv);
	cb_GetProcAddress(glGetQueryObjectuiv);
	cb_GetProcAddress(glBindBuffer);
	cb_GetProcAddress(glDeleteBuffers);
	cb_GetProcAddress(glGenBuffers);
	cb_GetProcAddress(glIsBuffer);
	cb_GetProcAddress(glBufferData);
	cb_GetProcAddress(glBufferSubData);
	cb_GetProcAddress(glGetBufferSubData);
	cb_GetProcAddress(glMapBuffer);
	cb_GetProcAddress(glUnmapBuffer);
	cb_GetProcAddress(glGetBufferParameteriv);
	cb_GetProcAddress(glGetBufferPointerv);
#endif /* !GL_VERSION_1_5 */
	return load_result;
}

/* ====================================
 * InitOpenGL_2_0
 */
bool InitOpenGL_2_0() {
	bool load_result = false;
#ifdef GL_VERSION_2_0
	cb_GetProcAddress(glBlendEquationSeparate);
	cb_GetProcAddress(glDrawBuffers);
	cb_GetProcAddress(glStencilOpSeparate);
	cb_GetProcAddress(glStencilFuncSeparate);
	cb_GetProcAddress(glStencilMaskSeparate);
	cb_GetProcAddress(glAttachShader);
	cb_GetProcAddress(glBindAttribLocation);
	cb_GetProcAddress(glCompileShader);
	cb_GetProcAddress(glCreateProgram);
	cb_GetProcAddress(glCreateShader);
	cb_GetProcAddress(glDeleteProgram);
	cb_GetProcAddress(glDeleteShader);
	cb_GetProcAddress(glDetachShader);
	cb_GetProcAddress(glDisableVertexAttribArray);
	cb_GetProcAddress(glEnableVertexAttribArray);
	cb_GetProcAddress(glGetActiveAttrib);
	cb_GetProcAddress(glGetActiveUniform);
	cb_GetProcAddress(glGetAttachedShaders);
	cb_GetProcAddress(glGetAttribLocation);
	cb_GetProcAddress(glGetProgramiv);
	cb_GetProcAddress(glGetProgramInfoLog);
	cb_GetProcAddress(glGetShaderiv);
	cb_GetProcAddress(glGetShaderInfoLog);
	cb_GetProcAddress(glGetShaderSource);
	cb_GetProcAddress(glGetUniformLocation);
	cb_GetProcAddress(glGetUniformfv);
	cb_GetProcAddress(glGetUniformiv);
	cb_GetProcAddress(glGetVertexAttribdv);
	cb_GetProcAddress(glGetVertexAttribfv);
	cb_GetProcAddress(glGetVertexAttribiv);
	cb_GetProcAddress(glGetVertexAttribPointerv);
	cb_GetProcAddress(glIsProgram);
	cb_GetProcAddress(glIsShader);
	cb_GetProcAddress(glLinkProgram);
	cb_GetProcAddress(glShaderSource);
	cb_GetProcAddress(glUseProgram);
	cb_GetProcAddress(glUniform1f);
	cb_GetProcAddress(glUniform2f);
	cb_GetProcAddress(glUniform3f);
	cb_GetProcAddress(glUniform4f);
	cb_GetProcAddress(glUniform1i);
	cb_GetProcAddress(glUniform2i);
	cb_GetProcAddress(glUniform3i);
	cb_GetProcAddress(glUniform4i);
	cb_GetProcAddress(glUniform1fv);
	cb_GetProcAddress(glUniform2fv);
	cb_GetProcAddress(glUniform3fv);
	cb_GetProcAddress(glUniform4fv);
	cb_GetProcAddress(glUniform1iv);
	cb_GetProcAddress(glUniform2iv);
	cb_GetProcAddress(glUniform3iv);
	cb_GetProcAddress(glUniform4iv);
	cb_GetProcAddress(glUniformMatrix2fv);
	cb_GetProcAddress(glUniformMatrix3fv);
	cb_GetProcAddress(glUniformMatrix4fv);
	cb_GetProcAddress(glValidateProgram);
	cb_GetProcAddress(glVertexAttrib1d);
	cb_GetProcAddress(glVertexAttrib1dv);
	cb_GetProcAddress(glVertexAttrib1f);
	cb_GetProcAddress(glVertexAttrib1fv);
	cb_GetProcAddress(glVertexAttrib1s);
	cb_GetProcAddress(glVertexAttrib1sv);
	cb_GetProcAddress(glVertexAttrib2d);
	cb_GetProcAddress(glVertexAttrib2dv);
	cb_GetProcAddress(glVertexAttrib2f);
	cb_GetProcAddress(glVertexAttrib2fv);
	cb_GetProcAddress(glVertexAttrib2s);
	cb_GetProcAddress(glVertexAttrib2sv);
	cb_GetProcAddress(glVertexAttrib3d);
	cb_GetProcAddress(glVertexAttrib3dv);
	cb_GetProcAddress(glVertexAttrib3f);
	cb_GetProcAddress(glVertexAttrib3fv);
	cb_GetProcAddress(glVertexAttrib3s);
	cb_GetProcAddress(glVertexAttrib3sv);
	cb_GetProcAddress(glVertexAttrib4Nbv);
	cb_GetProcAddress(glVertexAttrib4Niv);
	cb_GetProcAddress(glVertexAttrib4Nsv);
	cb_GetProcAddress(glVertexAttrib4Nub);
	cb_GetProcAddress(glVertexAttrib4Nubv);
	cb_GetProcAddress(glVertexAttrib4Nuiv);
	cb_GetProcAddress(glVertexAttrib4Nusv);
	cb_GetProcAddress(glVertexAttrib4bv);
	cb_GetProcAddress(glVertexAttrib4d);
	cb_GetProcAddress(glVertexAttrib4dv);
	cb_GetProcAddress(glVertexAttrib4f);
	cb_GetProcAddress(glVertexAttrib4fv);
	cb_GetProcAddress(glVertexAttrib4iv);
	cb_GetProcAddress(glVertexAttrib4s);
	cb_GetProcAddress(glVertexAttrib4sv);
	cb_GetProcAddress(glVertexAttrib4ubv);
	cb_GetProcAddress(glVertexAttrib4uiv);
	cb_GetProcAddress(glVertexAttrib4usv);
	cb_GetProcAddress(glVertexAttribPointer);
#endif /* !GL_VERSION_2_0 */
	return load_result;
}

/* ====================================
 * InitOpenGL_2_1
 */
bool InitOpenGL_2_1() {
	bool load_result = false;
#ifdef GL_VERSION_2_1
	cb_GetProcAddress(glUniformMatrix2x3fv);
	cb_GetProcAddress(glUniformMatrix3x2fv);
	cb_GetProcAddress(glUniformMatrix2x4fv);
	cb_GetProcAddress(glUniformMatrix4x2fv);
	cb_GetProcAddress(glUniformMatrix3x4fv);
	cb_GetProcAddress(glUniformMatrix4x3fv);
#endif /* !GL_VERSION_2_1 */
	return load_result;
}

/* ====================================
 * InitOpenGL_3_0
 */
bool InitOpenGL_3_0() {
	bool load_result = false;
#ifdef GL_VERSION_3_0
	cb_GetProcAddress(glColorMaski);
	cb_GetProcAddress(glGetBooleani_v);
	cb_GetProcAddress(glGetIntegeri_v);
	cb_GetProcAddress(glEnablei);
	cb_GetProcAddress(glDisablei);
	cb_GetProcAddress(glIsEnabledi);
	cb_GetProcAddress(glBeginTransformFeedback);
	cb_GetProcAddress(glEndTransformFeedback);
	cb_GetProcAddress(glBindBufferRange);
	cb_GetProcAddress(glBindBufferBase);
	cb_GetProcAddress(glTransformFeedbackVaryings);
	cb_GetProcAddress(glGetTransformFeedbackVarying);
	cb_GetProcAddress(glClampColor);
	cb_GetProcAddress(glBeginConditionalRender);
	cb_GetProcAddress(glEndConditionalRender);
	cb_GetProcAddress(glVertexAttribIPointer);
	cb_GetProcAddress(glGetVertexAttribIiv);
	cb_GetProcAddress(glGetVertexAttribIuiv);
	cb_GetProcAddress(glVertexAttribI1i);
	cb_GetProcAddress(glVertexAttribI2i);
	cb_GetProcAddress(glVertexAttribI3i);
	cb_GetProcAddress(glVertexAttribI4i);
	cb_GetProcAddress(glVertexAttribI1ui);
	cb_GetProcAddress(glVertexAttribI2ui);
	cb_GetProcAddress(glVertexAttribI3ui);
	cb_GetProcAddress(glVertexAttribI4ui);
	cb_GetProcAddress(glVertexAttribI1iv);
	cb_GetProcAddress(glVertexAttribI2iv);
	cb_GetProcAddress(glVertexAttribI3iv);
	cb_GetProcAddress(glVertexAttribI4iv);
	cb_GetProcAddress(glVertexAttribI1uiv);
	cb_GetProcAddress(glVertexAttribI2uiv);
	cb_GetProcAddress(glVertexAttribI3uiv);
	cb_GetProcAddress(glVertexAttribI4uiv);
	cb_GetProcAddress(glVertexAttribI4bv);
	cb_GetProcAddress(glVertexAttribI4sv);
	cb_GetProcAddress(glVertexAttribI4ubv);
	cb_GetProcAddress(glVertexAttribI4usv);
	cb_GetProcAddress(glGetUniformuiv);
	cb_GetProcAddress(glBindFragDataLocation);
	cb_GetProcAddress(glGetFragDataLocation);
	cb_GetProcAddress(glUniform1ui);
	cb_GetProcAddress(glUniform2ui);
	cb_GetProcAddress(glUniform3ui);
	cb_GetProcAddress(glUniform4ui);
	cb_GetProcAddress(glUniform1uiv);
	cb_GetProcAddress(glUniform2uiv);
	cb_GetProcAddress(glUniform3uiv);
	cb_GetProcAddress(glUniform4uiv);
	cb_GetProcAddress(glTexParameterIiv);
	cb_GetProcAddress(glTexParameterIuiv);
	cb_GetProcAddress(glGetTexParameterIiv);
	cb_GetProcAddress(glGetTexParameterIuiv);
	cb_GetProcAddress(glClearBufferiv);
	cb_GetProcAddress(glClearBufferuiv);
	cb_GetProcAddress(glClearBufferfv);
	cb_GetProcAddress(glClearBufferfi);
	cb_GetProcAddress(glGetStringi);
	cb_GetProcAddress(glIsRenderbuffer);
	cb_GetProcAddress(glBindRenderbuffer);
	cb_GetProcAddress(glDeleteRenderbuffers);
	cb_GetProcAddress(glGenRenderbuffers);
	cb_GetProcAddress(glRenderbufferStorage);
	cb_GetProcAddress(glGetRenderbufferParameteriv);
	cb_GetProcAddress(glIsFramebuffer);
	cb_GetProcAddress(glBindFramebuffer);
	cb_GetProcAddress(glDeleteFramebuffers);
	cb_GetProcAddress(glGenFramebuffers);
	cb_GetProcAddress(glCheckFramebufferStatus);
	cb_GetProcAddress(glFramebufferTexture1D);
	cb_GetProcAddress(glFramebufferTexture2D);
	cb_GetProcAddress(glFramebufferTexture3D);
	cb_GetProcAddress(glFramebufferRenderbuffer);
	cb_GetProcAddress(glGetFramebufferAttachmentParameteriv);
	cb_GetProcAddress(glGenerateMipmap);
	cb_GetProcAddress(glBlitFramebuffer);
	cb_GetProcAddress(glRenderbufferStorageMultisample);
	cb_GetProcAddress(glFramebufferTextureLayer);
	cb_GetProcAddress(glMapBufferRange);
	cb_GetProcAddress(glFlushMappedBufferRange);
	cb_GetProcAddress(glBindVertexArray);
	cb_GetProcAddress(glDeleteVertexArrays);
	cb_GetProcAddress(glGenVertexArrays);
	cb_GetProcAddress(glIsVertexArray);
#endif /* !GL_VERSION_3_0 */
	return load_result;
}

/* ====================================
 * InitOpenGL_3_1
 */
bool InitOpenGL_3_1() {
	bool load_result = false;
#ifdef GL_VERSION_3_1
	cb_GetProcAddress(glDrawArraysInstanced);
	cb_GetProcAddress(glDrawElementsInstanced);
	cb_GetProcAddress(glTexBuffer);
	cb_GetProcAddress(glPrimitiveRestartIndex);
	cb_GetProcAddress(glCopyBufferSubData);
	cb_GetProcAddress(glGetUniformIndices);
	cb_GetProcAddress(glGetActiveUniformsiv);
	cb_GetProcAddress(glGetActiveUniformName);
	cb_GetProcAddress(glGetUniformBlockIndex);
	cb_GetProcAddress(glGetActiveUniformBlockiv);
	cb_GetProcAddress(glGetActiveUniformBlockName);
	cb_GetProcAddress(glUniformBlockBinding);
#endif /* !GL_VERSION_3_1 */
	return load_result;
}

/* ====================================
 * InitOpenGL_3_2
 */
bool InitOpenGL_3_2() {
	bool load_result = false;
#ifdef GL_VERSION_3_2
	cb_GetProcAddress(glDrawElementsBaseVertex);
	cb_GetProcAddress(glDrawRangeElementsBaseVertex);
	cb_GetProcAddress(glDrawElementsInstancedBaseVertex);
	cb_GetProcAddress(glMultiDrawElementsBaseVertex);
	cb_GetProcAddress(glProvokingVertex);
	cb_GetProcAddress(glFenceSync);
	cb_GetProcAddress(glIsSync);
	cb_GetProcAddress(glDeleteSync);
	cb_GetProcAddress(glClientWaitSync);
	cb_GetProcAddress(glWaitSync);
	cb_GetProcAddress(glGetInteger64v);
	cb_GetProcAddress(glGetSynciv);
	cb_GetProcAddress(glGetInteger64i_v);
	cb_GetProcAddress(glGetBufferParameteri64v);
	cb_GetProcAddress(glFramebufferTexture);
	cb_GetProcAddress(glTexImage2DMultisample);
	cb_GetProcAddress(glTexImage3DMultisample);
	cb_GetProcAddress(glGetMultisamplefv);
	cb_GetProcAddress(glSampleMaski);
#endif /* !GL_VERSION_3_2 */
	return load_result;
}

/* ====================================
 * InitOpenGL_3_3
 */
bool InitOpenGL_3_3() {
	bool load_result = false;
#ifdef GL_VERSION_3_3
	cb_GetProcAddress(glBindFragDataLocationIndexed);
	cb_GetProcAddress(glGetFragDataIndex);
	cb_GetProcAddress(glGenSamplers);
	cb_GetProcAddress(glDeleteSamplers);
	cb_GetProcAddress(glIsSampler);
	cb_GetProcAddress(glBindSampler);
	cb_GetProcAddress(glSamplerParameteri);
	cb_GetProcAddress(glSamplerParameteriv);
	cb_GetProcAddress(glSamplerParameterf);
	cb_GetProcAddress(glSamplerParameterfv);
	cb_GetProcAddress(glSamplerParameterIiv);
	cb_GetProcAddress(glSamplerParameterIuiv);
	cb_GetProcAddress(glGetSamplerParameteriv);
	cb_GetProcAddress(glGetSamplerParameterIiv);
	cb_GetProcAddress(glGetSamplerParameterfv);
	cb_GetProcAddress(glGetSamplerParameterIuiv);
	cb_GetProcAddress(glQueryCounter);
	cb_GetProcAddress(glGetQueryObjecti64v);
	cb_GetProcAddress(glGetQueryObjectui64v);
	cb_GetProcAddress(glVertexAttribDivisor);
	cb_GetProcAddress(glVertexAttribP1ui);
	cb_GetProcAddress(glVertexAttribP1uiv);
	cb_GetProcAddress(glVertexAttribP2ui);
	cb_GetProcAddress(glVertexAttribP2uiv);
	cb_GetProcAddress(glVertexAttribP3ui);
	cb_GetProcAddress(glVertexAttribP3uiv);
	cb_GetProcAddress(glVertexAttribP4ui);
	cb_GetProcAddress(glVertexAttribP4uiv);
#endif /* !GL_VERSION_3_3 */
	return load_result;
}

/* ====================================
 * InitOpenGL_4_0
 */
bool InitOpenGL_4_0() {
	bool load_result = false;
#ifdef GL_VERSION_4_0
	cb_GetProcAddress(glMinSampleShading);
	cb_GetProcAddress(glBlendEquationi);
	cb_GetProcAddress(glBlendEquationSeparatei);
	cb_GetProcAddress(glBlendFunci);
	cb_GetProcAddress(glBlendFuncSeparatei);
	cb_GetProcAddress(glDrawArraysIndirect);
	cb_GetProcAddress(glDrawElementsIndirect);
	cb_GetProcAddress(glUniform1d);
	cb_GetProcAddress(glUniform2d);
	cb_GetProcAddress(glUniform3d);
	cb_GetProcAddress(glUniform4d);
	cb_GetProcAddress(glUniform1dv);
	cb_GetProcAddress(glUniform2dv);
	cb_GetProcAddress(glUniform3dv);
	cb_GetProcAddress(glUniform4dv);
	cb_GetProcAddress(glUniformMatrix2dv);
	cb_GetProcAddress(glUniformMatrix3dv);
	cb_GetProcAddress(glUniformMatrix4dv);
	cb_GetProcAddress(glUniformMatrix2x3dv);
	cb_GetProcAddress(glUniformMatrix2x4dv);
	cb_GetProcAddress(glUniformMatrix3x2dv);
	cb_GetProcAddress(glUniformMatrix3x4dv);
	cb_GetProcAddress(glUniformMatrix4x2dv);
	cb_GetProcAddress(glUniformMatrix4x3dv);
	cb_GetProcAddress(glGetUniformdv);
	cb_GetProcAddress(glGetSubroutineUniformLocation);
	cb_GetProcAddress(glGetSubroutineIndex);
	cb_GetProcAddress(glGetActiveSubroutineUniformiv);
	cb_GetProcAddress(glGetActiveSubroutineUniformName);
	cb_GetProcAddress(glGetActiveSubroutineName);
	cb_GetProcAddress(glUniformSubroutinesuiv);
	cb_GetProcAddress(glGetUniformSubroutineuiv);
	cb_GetProcAddress(glGetProgramStageiv);
	cb_GetProcAddress(glPatchParameteri);
	cb_GetProcAddress(glPatchParameterfv);
	cb_GetProcAddress(glBindTransformFeedback);
	cb_GetProcAddress(glDeleteTransformFeedbacks);
	cb_GetProcAddress(glGenTransformFeedbacks);
	cb_GetProcAddress(glIsTransformFeedback);
	cb_GetProcAddress(glPauseTransformFeedback);
	cb_GetProcAddress(glResumeTransformFeedback);
	cb_GetProcAddress(glDrawTransformFeedback);
	cb_GetProcAddress(glDrawTransformFeedbackStream);
	cb_GetProcAddress(glBeginQueryIndexed);
	cb_GetProcAddress(glEndQueryIndexed);
	cb_GetProcAddress(glGetQueryIndexediv);
#endif /* !GL_VERSION_4_0 */
	return load_result;
}

/* ====================================
 * InitOpenGL_4_1
 */
bool InitOpenGL_4_1() {
	bool load_result = false;
#ifdef GL_VERSION_4_1
	cb_GetProcAddress(glReleaseShaderCompiler);
	cb_GetProcAddress(glShaderBinary);
	cb_GetProcAddress(glGetShaderPrecisionFormat);
	cb_GetProcAddress(glDepthRangef);
	cb_GetProcAddress(glClearDepthf);
	cb_GetProcAddress(glGetProgramBinary);
	cb_GetProcAddress(glProgramBinary);
	cb_GetProcAddress(glProgramParameteri);
	cb_GetProcAddress(glUseProgramStages);
	cb_GetProcAddress(glActiveShaderProgram);
	cb_GetProcAddress(glCreateShaderProgramv);
	cb_GetProcAddress(glBindProgramPipeline);
	cb_GetProcAddress(glDeleteProgramPipelines);
	cb_GetProcAddress(glGenProgramPipelines);
	cb_GetProcAddress(glIsProgramPipeline);
	cb_GetProcAddress(glGetProgramPipelineiv);
	cb_GetProcAddress(glProgramUniform1i);
	cb_GetProcAddress(glProgramUniform1iv);
	cb_GetProcAddress(glProgramUniform1f);
	cb_GetProcAddress(glProgramUniform1fv);
	cb_GetProcAddress(glProgramUniform1d);
	cb_GetProcAddress(glProgramUniform1dv);
	cb_GetProcAddress(glProgramUniform1ui);
	cb_GetProcAddress(glProgramUniform1uiv);
	cb_GetProcAddress(glProgramUniform2i);
	cb_GetProcAddress(glProgramUniform2iv);
	cb_GetProcAddress(glProgramUniform2f);
	cb_GetProcAddress(glProgramUniform2fv);
	cb_GetProcAddress(glProgramUniform2d);
	cb_GetProcAddress(glProgramUniform2dv);
	cb_GetProcAddress(glProgramUniform2ui);
	cb_GetProcAddress(glProgramUniform2uiv);
	cb_GetProcAddress(glProgramUniform3i);
	cb_GetProcAddress(glProgramUniform3iv);
	cb_GetProcAddress(glProgramUniform3f);
	cb_GetProcAddress(glProgramUniform3fv);
	cb_GetProcAddress(glProgramUniform3d);
	cb_GetProcAddress(glProgramUniform3dv);
	cb_GetProcAddress(glProgramUniform3ui);
	cb_GetProcAddress(glProgramUniform3uiv);
	cb_GetProcAddress(glProgramUniform4i);
	cb_GetProcAddress(glProgramUniform4iv);
	cb_GetProcAddress(glProgramUniform4f);
	cb_GetProcAddress(glProgramUniform4fv);
	cb_GetProcAddress(glProgramUniform4d);
	cb_GetProcAddress(glProgramUniform4dv);
	cb_GetProcAddress(glProgramUniform4ui);
	cb_GetProcAddress(glProgramUniform4uiv);
	cb_GetProcAddress(glProgramUniformMatrix2fv);
	cb_GetProcAddress(glProgramUniformMatrix3fv);
	cb_GetProcAddress(glProgramUniformMatrix4fv);
	cb_GetProcAddress(glProgramUniformMatrix2dv);
	cb_GetProcAddress(glProgramUniformMatrix3dv);
	cb_GetProcAddress(glProgramUniformMatrix4dv);
	cb_GetProcAddress(glProgramUniformMatrix2x3fv);
	cb_GetProcAddress(glProgramUniformMatrix3x2fv);
	cb_GetProcAddress(glProgramUniformMatrix2x4fv);
	cb_GetProcAddress(glProgramUniformMatrix4x2fv);
	cb_GetProcAddress(glProgramUniformMatrix3x4fv);
	cb_GetProcAddress(glProgramUniformMatrix4x3fv);
	cb_GetProcAddress(glProgramUniformMatrix2x3dv);
	cb_GetProcAddress(glProgramUniformMatrix3x2dv);
	cb_GetProcAddress(glProgramUniformMatrix2x4dv);
	cb_GetProcAddress(glProgramUniformMatrix4x2dv);
	cb_GetProcAddress(glProgramUniformMatrix3x4dv);
	cb_GetProcAddress(glProgramUniformMatrix4x3dv);
	cb_GetProcAddress(glValidateProgramPipeline);
	cb_GetProcAddress(glGetProgramPipelineInfoLog);
	cb_GetProcAddress(glVertexAttribL1d);
	cb_GetProcAddress(glVertexAttribL2d);
	cb_GetProcAddress(glVertexAttribL3d);
	cb_GetProcAddress(glVertexAttribL4d);
	cb_GetProcAddress(glVertexAttribL1dv);
	cb_GetProcAddress(glVertexAttribL2dv);
	cb_GetProcAddress(glVertexAttribL3dv);
	cb_GetProcAddress(glVertexAttribL4dv);
	cb_GetProcAddress(glVertexAttribLPointer);
	cb_GetProcAddress(glGetVertexAttribLdv);
	cb_GetProcAddress(glViewportArrayv);
	cb_GetProcAddress(glViewportIndexedf);
	cb_GetProcAddress(glViewportIndexedfv);
	cb_GetProcAddress(glScissorArrayv);
	cb_GetProcAddress(glScissorIndexed);
	cb_GetProcAddress(glScissorIndexedv);
	cb_GetProcAddress(glDepthRangeArrayv);
	cb_GetProcAddress(glDepthRangeIndexed);
	cb_GetProcAddress(glGetFloati_v);
	cb_GetProcAddress(glGetDoublei_v);
#endif /* !GL_VERSION_4_1 */
	return load_result;
}

/* ====================================
 * InitOpenGL_4_2
 */
bool InitOpenGL_4_2() {
	bool load_result = false;
#ifdef GL_VERSION_4_2
	cb_GetProcAddress(glDrawArraysInstancedBaseInstance);
	cb_GetProcAddress(glDrawElementsInstancedBaseInstance);
	cb_GetProcAddress(glDrawElementsInstancedBaseVertexBaseInstance);
	cb_GetProcAddress(glGetInternalformativ);
	cb_GetProcAddress(glGetActiveAtomicCounterBufferiv);
	cb_GetProcAddress(glBindImageTexture);
	cb_GetProcAddress(glMemoryBarrier);
	cb_GetProcAddress(glTexStorage1D);
	cb_GetProcAddress(glTexStorage2D);
	cb_GetProcAddress(glTexStorage3D);
	cb_GetProcAddress(glDrawTransformFeedbackInstanced);
	cb_GetProcAddress(glDrawTransformFeedbackStreamInstanced);
#endif /* !GL_VERSION_4_2 */
	return load_result;
}

/* ====================================
 * InitOpenGL_4_3
 */
bool InitOpenGL_4_3() {
	bool load_result = false;
#ifdef GL_VERSION_4_3
	cb_GetProcAddress(glClearBufferData);
	cb_GetProcAddress(glClearBufferSubData);
	cb_GetProcAddress(glDispatchCompute);
	cb_GetProcAddress(glDispatchComputeIndirect);
	cb_GetProcAddress(glCopyImageSubData);
	cb_GetProcAddress(glFramebufferParameteri);
	cb_GetProcAddress(glGetFramebufferParameteriv);
	cb_GetProcAddress(glGetInternalformati64v);
	cb_GetProcAddress(glInvalidateTexSubImage);
	cb_GetProcAddress(glInvalidateTexImage);
	cb_GetProcAddress(glInvalidateBufferSubData);
	cb_GetProcAddress(glInvalidateBufferData);
	cb_GetProcAddress(glInvalidateFramebuffer);
	cb_GetProcAddress(glInvalidateSubFramebuffer);
	cb_GetProcAddress(glMultiDrawArraysIndirect);
	cb_GetProcAddress(glMultiDrawElementsIndirect);
	cb_GetProcAddress(glGetProgramInterfaceiv);
	cb_GetProcAddress(glGetProgramResourceIndex);
	cb_GetProcAddress(glGetProgramResourceName);
	cb_GetProcAddress(glGetProgramResourceiv);
	cb_GetProcAddress(glGetProgramResourceLocation);
	cb_GetProcAddress(glGetProgramResourceLocationIndex);
	cb_GetProcAddress(glShaderStorageBlockBinding);
	cb_GetProcAddress(glTexBufferRange);
	cb_GetProcAddress(glTexStorage2DMultisample);
	cb_GetProcAddress(glTexStorage3DMultisample);
	cb_GetProcAddress(glTextureView);
	cb_GetProcAddress(glBindVertexBuffer);
	cb_GetProcAddress(glVertexAttribFormat);
	cb_GetProcAddress(glVertexAttribIFormat);
	cb_GetProcAddress(glVertexAttribLFormat);
	cb_GetProcAddress(glVertexAttribBinding);
	cb_GetProcAddress(glVertexBindingDivisor);
	cb_GetProcAddress(glDebugMessageControl);
	cb_GetProcAddress(glDebugMessageInsert);
	cb_GetProcAddress(glDebugMessageCallback);
	cb_GetProcAddress(glGetDebugMessageLog);
	cb_GetProcAddress(glPushDebugGroup);
	cb_GetProcAddress(glPopDebugGroup);
	cb_GetProcAddress(glObjectLabel);
	cb_GetProcAddress(glGetObjectLabel);
	cb_GetProcAddress(glObjectPtrLabel);
	cb_GetProcAddress(glGetObjectPtrLabel);
#endif /* !GL_VERSION_4_3 */
	return load_result;
}

/* ====================================
 * InitOpenGL_4_4
 */
bool InitOpenGL_4_4() {
	bool load_result = false;
#ifdef GL_VERSION_4_4
	cb_GetProcAddress(glBufferStorage);
	cb_GetProcAddress(glClearTexImage);
	cb_GetProcAddress(glClearTexSubImage);
	cb_GetProcAddress(glBindBuffersBase);
	cb_GetProcAddress(glBindBuffersRange);
	cb_GetProcAddress(glBindTextures);
	cb_GetProcAddress(glBindSamplers);
	cb_GetProcAddress(glBindImageTextures);
	cb_GetProcAddress(glBindVertexBuffers);
#endif /* !GL_VERSION_4_4 */
	return load_result;
}

/* ====================================
 * InitOpenGL_4_5
 */
bool InitOpenGL_4_5() {
	bool load_result = false;
#ifdef GL_VERSION_4_5
	cb_GetProcAddress(glClipControl);
	cb_GetProcAddress(glCreateTransformFeedbacks);
	cb_GetProcAddress(glTransformFeedbackBufferBase);
	cb_GetProcAddress(glTransformFeedbackBufferRange);
	cb_GetProcAddress(glGetTransformFeedbackiv);
	cb_GetProcAddress(glGetTransformFeedbacki_v);
	cb_GetProcAddress(glGetTransformFeedbacki64_v);
	cb_GetProcAddress(glCreateBuffers);
	cb_GetProcAddress(glNamedBufferStorage);
	cb_GetProcAddress(glNamedBufferData);
	cb_GetProcAddress(glNamedBufferSubData);
	cb_GetProcAddress(glCopyNamedBufferSubData);
	cb_GetProcAddress(glClearNamedBufferData);
	cb_GetProcAddress(glClearNamedBufferSubData);
	cb_GetProcAddress(glMapNamedBuffer);
	cb_GetProcAddress(glMapNamedBufferRange);
	cb_GetProcAddress(glUnmapNamedBuffer);
	cb_GetProcAddress(glFlushMappedNamedBufferRange);
	cb_GetProcAddress(glGetNamedBufferParameteriv);
	cb_GetProcAddress(glGetNamedBufferParameteri64v);
	cb_GetProcAddress(glGetNamedBufferPointerv);
	cb_GetProcAddress(glGetNamedBufferSubData);
	cb_GetProcAddress(glCreateFramebuffers);
	cb_GetProcAddress(glNamedFramebufferRenderbuffer);
	cb_GetProcAddress(glNamedFramebufferParameteri);
	cb_GetProcAddress(glNamedFramebufferTexture);
	cb_GetProcAddress(glNamedFramebufferTextureLayer);
	cb_GetProcAddress(glNamedFramebufferDrawBuffer);
	cb_GetProcAddress(glNamedFramebufferDrawBuffers);
	cb_GetProcAddress(glNamedFramebufferReadBuffer);
	cb_GetProcAddress(glInvalidateNamedFramebufferData);
	cb_GetProcAddress(glInvalidateNamedFramebufferSubData);
	cb_GetProcAddress(glClearNamedFramebufferiv);
	cb_GetProcAddress(glClearNamedFramebufferuiv);
	cb_GetProcAddress(glClearNamedFramebufferfv);
	cb_GetProcAddress(glClearNamedFramebufferfi);
	cb_GetProcAddress(glBlitNamedFramebuffer);
	cb_GetProcAddress(glCheckNamedFramebufferStatus);
	cb_GetProcAddress(glGetNamedFramebufferParameteriv);
	cb_GetProcAddress(glGetNamedFramebufferAttachmentParameteriv);
	cb_GetProcAddress(glCreateRenderbuffers);
	cb_GetProcAddress(glNamedRenderbufferStorage);
	cb_GetProcAddress(glNamedRenderbufferStorageMultisample);
	cb_GetProcAddress(glGetNamedRenderbufferParameteriv);
	cb_GetProcAddress(glCreateTextures);
	cb_GetProcAddress(glTextureBuffer);
	cb_GetProcAddress(glTextureBufferRange);
	cb_GetProcAddress(glTextureStorage1D);
	cb_GetProcAddress(glTextureStorage2D);
	cb_GetProcAddress(glTextureStorage3D);
	cb_GetProcAddress(glTextureStorage2DMultisample);
	cb_GetProcAddress(glTextureStorage3DMultisample);
	cb_GetProcAddress(glTextureSubImage1D);
	cb_GetProcAddress(glTextureSubImage2D);
	cb_GetProcAddress(glTextureSubImage3D);
	cb_GetProcAddress(glCompressedTextureSubImage1D);
	cb_GetProcAddress(glCompressedTextureSubImage2D);
	cb_GetProcAddress(glCompressedTextureSubImage3D);
	cb_GetProcAddress(glCopyTextureSubImage1D);
	cb_GetProcAddress(glCopyTextureSubImage2D);
	cb_GetProcAddress(glCopyTextureSubImage3D);
	cb_GetProcAddress(glTextureParameterf);
	cb_GetProcAddress(glTextureParameterfv);
	cb_GetProcAddress(glTextureParameteri);
	cb_GetProcAddress(glTextureParameterIiv);
	cb_GetProcAddress(glTextureParameterIuiv);
	cb_GetProcAddress(glTextureParameteriv);
	cb_GetProcAddress(glGenerateTextureMipmap);
	cb_GetProcAddress(glBindTextureUnit);
	cb_GetProcAddress(glGetTextureImage);
	cb_GetProcAddress(glGetCompressedTextureImage);
	cb_GetProcAddress(glGetTextureLevelParameterfv);
	cb_GetProcAddress(glGetTextureLevelParameteriv);
	cb_GetProcAddress(glGetTextureParameterfv);
	cb_GetProcAddress(glGetTextureParameterIiv);
	cb_GetProcAddress(glGetTextureParameterIuiv);
	cb_GetProcAddress(glGetTextureParameteriv);
	cb_GetProcAddress(glCreateVertexArrays);
	cb_GetProcAddress(glDisableVertexArrayAttrib);
	cb_GetProcAddress(glEnableVertexArrayAttrib);
	cb_GetProcAddress(glVertexArrayElementBuffer);
	cb_GetProcAddress(glVertexArrayVertexBuffer);
	cb_GetProcAddress(glVertexArrayVertexBuffers);
	cb_GetProcAddress(glVertexArrayAttribBinding);
	cb_GetProcAddress(glVertexArrayAttribFormat);
	cb_GetProcAddress(glVertexArrayAttribIFormat);
	cb_GetProcAddress(glVertexArrayAttribLFormat);
	cb_GetProcAddress(glVertexArrayBindingDivisor);
	cb_GetProcAddress(glGetVertexArrayiv);
	cb_GetProcAddress(glGetVertexArrayIndexediv);
	cb_GetProcAddress(glGetVertexArrayIndexed64iv);
	cb_GetProcAddress(glCreateSamplers);
	cb_GetProcAddress(glCreateProgramPipelines);
	cb_GetProcAddress(glCreateQueries);
	cb_GetProcAddress(glGetQueryBufferObjecti64v);
	cb_GetProcAddress(glGetQueryBufferObjectiv);
	cb_GetProcAddress(glGetQueryBufferObjectui64v);
	cb_GetProcAddress(glGetQueryBufferObjectuiv);
	cb_GetProcAddress(glMemoryBarrierByRegion);
	cb_GetProcAddress(glGetTextureSubImage);
	cb_GetProcAddress(glGetCompressedTextureSubImage);
	cb_GetProcAddress(glGetGraphicsResetStatus);
	cb_GetProcAddress(glGetnCompressedTexImage);
	cb_GetProcAddress(glGetnTexImage);
	cb_GetProcAddress(glGetnUniformdv);
	cb_GetProcAddress(glGetnUniformfv);
	cb_GetProcAddress(glGetnUniformiv);
	cb_GetProcAddress(glGetnUniformuiv);
	cb_GetProcAddress(glReadnPixels);
	cb_GetProcAddress(glTextureBarrier);
#endif /* !GL_VERSION_4_5 */
	return load_result;
}

/* ====================================
 * InitOpenGL_4_6
 */
bool InitOpenGL_4_6() {
	bool load_result = false;
#ifdef GL_VERSION_4_6
	cb_GetProcAddress(glSpecializeShader);
	cb_GetProcAddress(glMultiDrawArraysIndirectCount);
	cb_GetProcAddress(glMultiDrawElementsIndirectCount);
	cb_GetProcAddress(glPolygonOffsetClamp);
#endif /* !GL_VERSION_4_6 */
	return load_result;
}

#define cb_LoadGL(MAJOR, MINOR) { load_result = load_result | InitOpenGL_##MAJOR##_##MINOR(); if(!load_result){ return false; } }

bool cbpp::LoadGL() {
	FILE* tmp = fopen("logs/OpenGL_load.txt", "w");
	fclose(tmp);
	
	#ifdef __linux__
	_linux_libGL_handle = dlopen("libGL.so", RTLD_NOW | RTLD_GLOBAL);
	#endif

	bool load_result = false;
	cb_LoadGL(1, 0)
	cb_LoadGL(1, 1)
	cb_LoadGL(1, 2)
	cb_LoadGL(1, 3)
	cb_LoadGL(1, 4)
	cb_LoadGL(1, 5)
	
	cb_LoadGL(2, 0)
	cb_LoadGL(2, 1)
	
	cb_LoadGL(3, 0)
	cb_LoadGL(3, 1)
	cb_LoadGL(3, 2)
	cb_LoadGL(3, 3)
	
	cb_LoadGL(4, 0)
	//cb_LoadGL(4, 1)
	//cb_LoadGL(4, 2)
	//cb_LoadGL(4, 3)
	//cb_LoadGL(4, 4)
	//cb_LoadGL(4, 5)
	//cb_LoadGL(4, 6)

	#ifdef __linux
	if(_linux_libGL_handle != NULL) { dlclose(_linux_libGL_handle); }
	#endif
	
	return load_result;
}
