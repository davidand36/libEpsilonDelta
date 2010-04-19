TestCGILib is a CGI/FastCGI program, and needs to be tested using an HTTP
server, such as Apache. I run an instance of Apache on my dev machine with
mod_fcgid installed and enabled, which is only accessable from the same
machine. On this I have a configuration something like this:

<VirtualHost *:80>
    DocumentRoot  var/www/htdocs
    ScriptAlias  /bin/  /var/www/cgi-bin/
    ScriptAlias  /fcgi/  /var/www/fcgi/
    <Directory /var/www/cgi-bin>
        Options  ExecCGI
    </Directory>
    <Directory /var/www/fcgi>
        Options ExecCGI SymLinksIfOwnerMatch
        SetHandler fcgid-script
    </Directory>
</VirtualHost>

I copy TestCGILib to /var/www/cgi-bin, and put a link to that file in
/var/www/fcgi. I copy test_cgi.html into /var/www/htdocs. 
In a browser I go to localhost/test_cgi.html, where I see forms that let me
exercise the system. As a file to upload, I have provided TurkishAlphabet.txt,
which is small and also provides a test of the browser's (and server's)
handling of non-ASCII UTF-8.

