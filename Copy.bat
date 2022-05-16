cd..
cd..

xcopy .\GameEngine\Include\*.h .\Engine\Include\ /s /d /y
xcopy .\GameEngine\Include\*.inl .\Engine\Include\ /s /d /y
xcopy .\GameEngine\Include\*.hpp .\Engine\Include\ /s /d /y

xcopy .\GameEngine\Bin\*.lib .\Engine\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.lib .\Client2D\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.lib .\GameEditor\Bin\ /s /d /y

xcopy .\GameEngine\Bin\*.dll .\Engine\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.dll .\Client2D\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.dll .\GameEditor\Bin\ /s /d /y

xcopy .\GameEngine\Bin\Shader\ .\Engine\Bin\Shader\ /s /d /y
xcopy .\GameEngine\Bin\Shader\ .\Client2D\Bin\Shader\ /s /d /y
xcopy .\GameEngine\Bin\Shader\ .\GameEditor\Bin\Shader\ /s /d /y

xcopy .\GameEngine\Bin\Texture\ .\Engine\Bin\Texture\ /s /d /y
xcopy .\GameEngine\Bin\Texture\ .\Client2D\Bin\Texture\ /s /d /y
xcopy .\GameEngine\Bin\Texture\ .\GameEditor\Bin\Texture\ /s /d /y

xcopy .\GameEngine\Bin\Sound\ .\Engine\Bin\Sound\ /s /d /y
xcopy .\GameEngine\Bin\Sound\ .\Client2D\Bin\Sound\ /s /d /y
xcopy .\GameEngine\Bin\Sound\ .\GameEditor\Bin\Sound\ /s /d /y

xcopy .\GameEngine\Bin\Font\ .\Engine\Bin\Font\ /s /d /y
xcopy .\GameEngine\Bin\Font\ .\Client2D\Bin\Font\ /s /d /y
xcopy .\GameEngine\Bin\Font\ .\GameEditor\Bin\Font\ /s /d /y