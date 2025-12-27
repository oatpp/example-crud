ALTER TABLE AppUser
ADD COLUMN created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP NOT NULL,
ADD COLUMN updated_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP NOT NULL,
ADD COLUMN deleted_at TIMESTAMPTZ DEFAULT NULL;

CREATE OR REPLACE FUNCTION get_user_by_id(p_id INT)
RETURNS TABLE (
    id INT,
    username TEXT,
    email TEXT,
    role TEXT,
    created_at BIGINT,
    updated_at BIGINT
) AS $$
    SELECT u.id, u.username, u.email, u.role, EXTRACT(EPOCH FROM u.created_at)::BIGINT, EXTRACT(EPOCH FROM u.updated_at)::BIGINT
    FROM AppUser u
    WHERE u.id = p_id
    LIMIT 1;
$$ LANGUAGE sql;

CREATE OR REPLACE FUNCTION get_user_by_username(p_username TEXT)
RETURNS TABLE (
    id INT,
    username TEXT,
    password TEXT,
    email TEXT,
    role TEXT,
    created_at BIGINT,
    updated_at BIGINT
) AS $$
    SELECT u.id, u.username, u.password, u.email, u.role, EXTRACT(EPOCH FROM u.created_at)::BIGINT, EXTRACT(EPOCH FROM u.updated_at)::BIGINT
    FROM AppUser u
    WHERE u.username = p_username
    LIMIT 1;
$$ LANGUAGE sql;