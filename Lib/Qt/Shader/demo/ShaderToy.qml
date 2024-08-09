// uniform vec3      iResolution;           // viewport resolution (in pixels)
// uniform float     iTime;                 // shader playback time (in seconds)
// uniform float     iChannelTime[4];       // channel playback time (in seconds)
// uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
// uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
// uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
// uniform vec4      iDate;                 // (year, month, day, time in seconds)
// uniform float     iSampleRate;           // sound sample rate (i.e., 44100)
// [Example]
// iChannel0:  ShaderEffectSource {
//     wrapMode: ShaderEffectSource.Repeat
//     recursive: true
//     mipmap: true
//     sourceItem: Image {
//         source: "./example/textimage/ichannel0.png"
//     }
// }
// [Example]
// NumberAnimation on iTime { loops: Animation.Infinite; from: 0; to: Math.PI * 2; duration: 6914/4 }

import QtQuick 2.4

ShaderEffect {
    id: shader

    property url base: "https://www.shadertoy.com/view/?"
    property string title
    property string describe
    readonly property vector3d iResolution: Qt.vector3d(shader.width, shader.height, 0)
    property int iFrame: 10
    property var iChannelTime: [0, 1, 2, 3]
    property var iChannelResolution: [Qt.vector3d(shader.width, shader.height, 0)]
    property var iChannel0
    property var iChannel1
    property var iChannel2
    property var iChannel3
    property real iTime: 0
    property real iTimeDelta: 100
    property vector4d iDate
    property real iSampleRate: 4410
    property vector4d iMouse
    property alias hoverEnabled: mouse.hoverEnabled
    property string pixelShader: ""

    vertexShader: "
        uniform highp mat4 qt_Matrix;
        attribute highp vec4 qt_Vertex;
        attribute highp vec2 qt_MultiTexCoord0;
        varying highp vec2 coord;
        void main() {
            coord = qt_MultiTexCoord0;
            gl_Position = qt_Matrix * qt_Vertex;
        }
    "
    fragmentShader: "
        #ifndef GL_ES
        #extension GL_EXT_shader_texture_lod : enable
        #extension GL_OES_standard_derivatives : enable
        precision highp float;
        precision highp int;
        precision mediump sampler2D;
        #endif

        #ifdef GL_ES
        precision mediump float;
        #endif

        uniform lowp float qt_Opacity;
        varying highp vec2 qt_TexCoord0;

        uniform vec3 iResolution ;

        uniform float iTime;

        uniform float     iChannelTime[4];
        uniform vec3      iChannelResolution[4];

        uniform vec4      iMouse;
        uniform sampler2D iChannel0;
        uniform sampler2D iChannel1;
        uniform sampler2D iChannel2;
        uniform sampler2D iChannel3;
        uniform vec4      iDate;

        uniform float     iSampleRate;

        void mainImage( out vec4 fragColor, in vec2 fragCoord );

        void main(void)
        {
            mainImage(gl_FragColor, gl_FragCoord.xy);
        }
    " + pixelShader

    Timer {
        id: timer

        running: true
        interval: 1000
        onTriggered: {
            // 更新 iDate
            var date = new Date();
            shader.iDate.x = date.getFullYear();
            shader.iDate.y = date.getMonth();
            shader.iDate.z = date.getDay();
            shader.iDate.w = date.getSeconds();
        }
    }

    MouseArea {
        id: mouse

        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onPositionChanged: {
            shader.iMouse.x = mouseX;
            shader.iMouse.y = mouseY;
        }
        onClicked: {
            shader.iMouse.z = mouseX;
            shader.iMouse.w = mouseY;
        }
    }
}
