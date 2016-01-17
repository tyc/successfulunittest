This is some instructions on how to run the backup.

On the website of successfulunittest.com, there is a script to
backup the wordpress sites and database. It is executed by the 
script found at 

http://www.guyrutenberg.com/2010/02/28/improved-ftp-backup-for-wordpress/

This script also perform an ftp transfer of the .bz2 files to a server,
but I have commented it out. So for me, it just create the .bz2 files.

To get the files out of successfulunittest.com, sftp into it and transfer
it onto the local machine.

On the home directory, there is a directory called wpback. In there, the
script that I mentioned above lives there. It is ready to execute.

Call it as the following

./wpbackup /var/www/html

It will create two .bz2 files in the directory backup that are date stamped.
It only has a resolution of days, so if you run the script more than once a
day, the earlier ones will be overwritten.

There is a vsftpd on successfulunittest.com, so sftp into it and download
it.


 
