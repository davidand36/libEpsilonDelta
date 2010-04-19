TestSQLLib will require access to PostgreSQL and/or MySQL databases, if you
have enabled their use. I run local instances of each on my dev machine,
running Ubuntu Linux. Here's how I set them up:

PostgreSQL:
$ sudo -u postgres psql postgres
# \password postgres (and then set a password for this superuser)
# \q
$ sudo -u postgres createuser --superuser <myid>
$ sudo -u postgres psql
# \password <myid> (and set a password for me as a superuser)
# \q
$ createdb <myid>
(Now I can just type "psql" to enter the PostgreSQL shell.)
$ createuser -D -A -P testuser (set password, e.g., "TestPW")
$ createdb -O testuser testdb

MySQL:
$ mysql -u root -p (and enter password set during installation)
> GRANT ALL ON *.* TO <myid>@localhost IDENTIFIED BY '<mypw>' WITH GRANT OPTION;
> quit
$ mysql -p
> CREATE DATABASE testdb;
> GRANT CREATE,DROP,ALTER,INSERT,DELETE,UPDATE,SELECT,INDEX ON testdb TO testuser IDENTIFIED BY 'TestPW';
> quit

Now create sql/test/PostgreSQL.conf and/or sql/test/MySQL.config files:
host=localhost
database=testdb
user=testuser
password=TestPW

