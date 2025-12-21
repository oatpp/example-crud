
CREATE TABLE AppUser (
    id          SERIAL PRIMARY KEY,
    username    VARCHAR UNIQUE,
    email       VARCHAR,
    password    VARCHAR,
    role        VARCHAR
);

INSERT INTO AppUser
(username, email, password, role) VALUES ('admin', 'admin@domain.com', 'admin', 'ROLE_ADMIN');
