//#############################################################################
//  File:      SLSkybox
//  Author:    Marcus Hudritsch
//  Date:      December 2017
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#include <stdafx.h>           // precompiled headers
#ifdef SL_MEMLEAKDETECT       // set in SL.h for debug config only
#include <debug_new.h>        // memory leak detector
#endif

#include <SLSkybox.h>
#include <SLGLTexture.h>
#include <SLGLFrameBuffer.h>
#include <SLGLRenderBuffer.h>
#include <SLMaterial.h>
#include <SLBox.h>
#include <SLCamera.h>
#include <SLSceneView.h>

//-----------------------------------------------------------------------------
//! Default constructor
SLSkybox::SLSkybox(SLstring name) : SLNode(name)
{

}
//-----------------------------------------------------------------------------
//! Cubemap Constructor with cubemap images
/*! All resources allocated are stored in the SLScene vectors for textures,
materials, programs and meshes and get deleted at scene destruction.
*/
SLSkybox::SLSkybox(SLstring cubeMapXPos,
                   SLstring cubeMapXNeg,
                   SLstring cubeMapYPos,
                   SLstring cubeMapYNeg,
                   SLstring cubeMapZPos,
                   SLstring cubeMapZNeg,
                   SLstring name) : SLNode(name)
{
    // Create texture, material and program
    SLGLTexture* cubeMap = new SLGLTexture(cubeMapXPos,cubeMapXNeg
                                          ,cubeMapYPos,cubeMapYNeg
                                          ,cubeMapZPos,cubeMapZNeg);
    SLMaterial* matCubeMap = new SLMaterial("matCubeMap");
    matCubeMap->textures().push_back(cubeMap);
    SLGLProgram* sp = new SLGLGenericProgram("SkyBox.vert", "SkyBox.frag");
    matCubeMap->program(sp);

    // Create a box with max. point at min. parameter and vice versa.
    // Like this the boxes normals will point to the inside.
    this->addMesh(new SLBox(10,10,10, -10,-10,-10, "box", matCubeMap));
}
//-----------------------------------------------------------------------------
//! Draw the skybox with a cube map with the camera in its center.
SLSkybox::SLSkybox(SLScene* s,
                   SLstring hdrImage,
                   SLstring name) : SLNode(name)
{
    SLGLProgram* backgroundShader = new SLGLGenericProgram("Background.vert", "Background.frag");
    SLGLUniform1f* exposure = new SLGLUniform1f(UT_const, "u_exposure", 1.0f, 0.02f, 0.1, 10.0, (SLKey)'H');
    s->eventHandlers().push_back(exposure);
    backgroundShader->addUniform1f(exposure);
    
    SLGLTexture* envCubemap = new SLGLTexture(hdrImage,
                                              GL_LINEAR,
                                              GL_LINEAR,
                                              TT_hdr,
                                              GL_CLAMP_TO_EDGE,
                                              GL_CLAMP_TO_EDGE,
                                              1024,
                                              1024);

    SLMaterial* hdrMaterial = new SLMaterial("matCubeMap");
    hdrMaterial->textures().push_back(envCubemap);
    hdrMaterial->program(backgroundShader);

    this->addMesh(new SLBox(10,10,10,-10,-10,-10, "box", hdrMaterial));
}

//-----------------------------------------------------------------------------
//! Draw the skybox with a cube map with the camera in its center.
void SLSkybox::drawAroundCamera(SLSceneView* sv)
{
    assert(sv && "No SceneView passed to SLSkybox::drawAroundCamera");
    
    // Set the view transform
    _stateGL->modelViewMatrix.setMatrix(_stateGL->viewMatrix);

    // Put skybox at the cameras position
    this->translation(sv->camera()->translationWS());

    // Apply world transform
    _stateGL->modelViewMatrix.multiply(this->updateAndGetWM().m());

    // Freeze depth buffer
    _stateGL->depthMask(false);

    // Draw the box
    this->drawMeshes(sv);

    // Unlock depth buffer
    _stateGL->depthMask(true);
}
//-----------------------------------------------------------------------------
//! Returns the color in the skybox at the the specified direction dir
SLCol4f SLSkybox::colorAtDir(SLVec3f dir)
{
    assert(_meshes.size() > 0);
    assert(_meshes[0]->mat()->textures().size() > 0);
    
    SLGLTexture* tex = _meshes[0]->mat()->textures()[0];
    
    return tex->getTexelf(dir);
}
//-----------------------------------------------------------------------------
