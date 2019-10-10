/* -*-c++-*- OpenSceneGraph Cookbook
 * Chapter 3 Recipe 10
 * Author: Wang Rui <wangray84 at gmail dot com>
*/

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

#include "CommonFunctions"

#define ROWS 256
#define COLS 256
#define WIDTH 384.0f
#define AMPLITUDE 16.0f

#define to_string_i(s) #s
#define to_string(s) to_string_i(s)

const char* vertCode = {
    "#define ROWS " to_string(ROWS) " \n"
    "#define COLS " to_string(COLS) " \n"
    "#define WIDTH " to_string(WIDTH) " \n"
    "#define AMPLITUDE " to_string(AMPLITUDE) " \n"
    "uniform sampler2D defaultTex;\n"
    "const float PI2 = 6.2831852;\n"
    "void main()\n"
    "{\n"
    "    float r = float(gl_InstanceID) / ROWS;\n" // row
    "    vec2 uv = vec2(fract(r), floor(r) / ROWS);\n"
    // use any number that's greater than ROLS OR COLS to create gap between quads
    "    vec4 pos = gl_Vertex + vec4(uv.s * WIDTH, AMPLITUDE * sin(uv.s * PI2), uv.t * WIDTH, 0.0);\n"
    "    gl_FrontColor = texture2D(defaultTex, uv);\n"
    // "    gl_FrontColor = vec4(uv, 0, 1);\n"
    // "    gl_FrontColor = vec4(gl_ModelViewMatrix[2].xyz, 1);\n"
    "    gl_Position = gl_ModelViewProjectionMatrix * pos;\n"
    "}\n"
};

osg::Geometry* createInstancedGeometry( unsigned int numInstances )
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(4);
    (*vertices)[0].set(-0.5f, 0.0f,-0.5f );
    (*vertices)[1].set( 0.5f, 0.0f,-0.5f );
    (*vertices)[2].set( 0.5f, 0.0f, 0.5f );
    (*vertices)[3].set(-0.5f, 0.0f, 0.5f );
    
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->setUseDisplayList( false );
    geom->setUseVertexBufferObjects( true );
    geom->setVertexArray( vertices.get() );
    geom->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4, numInstances) );
    geom->setInitialBound( osg::BoundingBox(-1.0f,-AMPLITUDE,-1.0f, WIDTH, AMPLITUDE, WIDTH) );
    
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage( osgDB::readImageFile("Images/osg256.png") );
    texture->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
    texture->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );
    geom->getOrCreateStateSet()->setTextureAttributeAndModes( 0, texture.get() );
    geom->getOrCreateStateSet()->addUniform( new osg::Uniform("defaultTex", 0) );
    
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader( new osg::Shader(osg::Shader::VERTEX, vertCode) );
    geom->getOrCreateStateSet()->setAttributeAndModes( program.get() );


    return geom.release();
}

int main( int argc, char** argv )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( createInstancedGeometry(ROWS * COLS) );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( geode.get() );

    osgViewer::Viewer viewer;
    viewer.realize();
    viewer.setSceneData( root.get() );

    return viewer.run();
}
