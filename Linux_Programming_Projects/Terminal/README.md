This is a my built Terminal which can execute simple command like: ls, cd, wc,...
There are own commands which are exit, help, hello and specially cd which is not a built-in system command. 
The program is execute and choose action mainly through processString() fucntion.
This version is not capable for running own pipe '$' command for example: 
    - cmd1 $ cmd2 cmd3 (which output of     cmd1 is input to both cmd2 and cmd3)
    - cmd1 cmd2 $ cmd3 (output of cmd1 and cmd2 is input to cmd 3)
    - cmd1 cmd2 $ cmd3 cmd4 (output of cmd1 and cmd2 are input to both cmd3 and cmd4)
This version is also not capable for running command in background using '$'
