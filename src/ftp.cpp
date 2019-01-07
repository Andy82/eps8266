#include <ftp.h>
#include <ESP8266FtpServer.h>

FtpServer ftpSrv;

void Ftp_init()
{
    String ftpUser = jConfig.jRead("ftpUser");
    String ftpPassword = jConfig.jRead("ftpPassword");
    logger.log("FTP running. User: " + ftpUser + " Password: " + ftpPassword);
    ftpSrv.begin(ftpUser, ftpPassword); 
}


void Ftp_Loop()
{
    ftpSrv.handleFTP(); 
}