var fso = new ActiveXObject("Scripting.FileSystemObject");

if (fso.FileExists("test.txt")) {
    WScript.Echo("file exists");
} else {
    WScript.Echo("file not exists");
}
