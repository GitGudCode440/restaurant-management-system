FROM mariadb:latest

ENV MARIADB_ROOT_PASSWORD=12345 \
    MARIADB_DATABASE=restaurant_db

COPY schema.sql /docker-entrypoint-initdb.d/

EXPOSE 3306
