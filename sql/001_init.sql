
CREATE TABLE AppUser (
    id          SERIAL PRIMARY KEY,
    username    VARCHAR UNIQUE NOT NULL,
    email       VARCHAR NOT NULL,
    password    VARCHAR NOT NULL,
    role        VARCHAR NOT NULL DEFAULT 'ROLE_GUEST'
) with (oids=false);

INSERT INTO AppUser
(username, email, password, role) VALUES ('admin', 'admin@domain.com', 'admin', 'ROLE_ADMIN');
