#pragma once

enum AXIS
{
    AXIS_X,
    AXIS_Y,
    AXIS_Z,
    AXIS_END
};

enum class Mesh_Type
{
    Sprite,
    StaticMesh,
    AnimationMesh
};

enum class Shader_Type
{
    Graphic,
    Compute
};

enum class SceneComponent_Type
{
    Scene,
    Primitive,
    Camera,
    SpringArm
};

enum class ObjectComponent_Type
{
    PaperBurn
};

enum class PrimitiveComponent_Type
{
    Primitive2D,
    Primitive3D,
    PrimitiveWidget
};

enum Constant_Buffer_Type
{
    CBT_VERTEX = 0x1,
    CBT_PIXEL = 0x2,
    CBT_DOMAIN = 0x4,
    CBT_HULL = 0x8,
    CBT_GEOMETRY = 0x10,
    CBT_COMPUTE = 0x20,
    CBT_ALL_GRAPHIC = CBT_VERTEX | CBT_PIXEL | CBT_DOMAIN | CBT_HULL | CBT_GEOMETRY,
    CBT_ALL = CBT_ALL_GRAPHIC | CBT_COMPUTE
};

enum Texture_Shader_Type
{
    TST_VERTEX = 0x1,
    TST_PIXEL = 0x2,
    TST_DOMAIN = 0x4,
    TST_HULL = 0x8,
    TST_GEOMETRY = 0x10,
    TST_COMPUTE = 0x20,
    TST_ALL_GRAPHIC = TST_VERTEX | TST_PIXEL | TST_DOMAIN | TST_HULL | TST_GEOMETRY,
    TST_ALL = TST_ALL_GRAPHIC | TST_COMPUTE
};

enum Render_Type_2D
{
    RT2D_None = -1,
    RT2D_MAP,
    RT2D_Back,      // z : 0.7f ~ 0.99999f
    RT2D_Default,   // z : 0.3f ~ 0.69999f
    RT2D_Particle,  // z : 0.f ~ 0.29999f
    RT2D_End
};

enum Render_Type_3D
{
    RT3D_None = -1,
    RT3D_Default,
    RT3D_Particle,
    RT3D_End
};

enum class Input_Type
{
    Window,
    DirectInput
};

enum Key_Type
{
    KT_Down,
    KT_Push,
    KT_Up,
    KT_DBLClick,
    KT_End
};

enum Key_Public
{
    KP_Control,
    KP_Alt,
    KP_Shift,
    KP_End
};

enum class Image_Type
{
    Atlas,
    Frame,
    Array
};

enum class Texture_Link
{
    BaseTexture,        // 0번 레지스터
    EmissiveTexture,    // 1번 레지스터
    CustomTexture   // 이 옵션을 사용하면 레지스터를 원하는대로 지정해서 사용할 수 있다.
};

enum class Animation2D_Type
{
    Atlas,
    Frame,
    Array
};

enum RenderState_Type
{
    RST_Blend,
    RST_DepthStencil,
    RST_Rasterizer,
    RST_End
};

enum class Camera_Type
{
    Cam3D,
    Cam2D,
    CamUI
};

enum class Collider_Space
{
    Collider2D,
    Collider3D
};

enum class Collider_Shape
{
    Box2D,
    Circle,
    Pixel
};

enum class Render_Space
{
    Render2D,
    Render3D
};

enum class Collision_Channel
{
    Static,
    Player,
    Monster,
    Custom1,
    Custom2,
    Custom3,
    Custom4,
    Custom5,
    Custom6,
    Custom7,
    Custom8,
    Custom9,
    Custom10,
    Custom11,
    Custom12,
    Custom13,
    Custom14,
    Custom15,
    Custom16,
    Custom17,
    Custom18,
    End
};

enum class Collision_Interaction
{
    Ignore,
    Block
};

enum class Collision_State
{
    Begin,
    End,
    Max
};

enum class PixelCollision_Type
{
    Color_Ignore,
    Color_Confirm,
    Alpha_Ignore,
    Alpha_Confirm
};

enum class Component_Class_Type
{
    Scene,
    Sprite,
    Box2D,
    Circle,
    Pixel,
    Camera,
    SpringArm,
    SpringArm2D,
    ParticleSystem,
    End
};

enum class Button_State
{
    Normal,
    MouseOn,
    Click,
    Disabled,
    End
};

enum class Button_Sound_State
{
    MouseOn,
    Click,
    End
};

enum class ProgressBar_Dir
{
    RightToLeft,
    LeftToRight,
    TopToBottom,
    BottomToTop
};

enum class WidgetComponent_Space
{
    Space2D,
    Space3D
};

enum class Tile_Shape
{
    Rect,
    Rhombus
};

enum class Tile_Type
{
    None,
    Wall
};
