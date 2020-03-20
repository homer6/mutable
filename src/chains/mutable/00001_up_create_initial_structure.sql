

CREATE TABLE chains(
	id SERIAL PRIMARY KEY,
	name VARCHAR(50) UNIQUE NOT NULL,
	description VARCHAR(50) NULL,
	admin_email VARCHAR(355) NOT NULL,
	created_at TIMESTAMP NOT NULL,
	updated_at TIMESTAMP
);


CREATE TABLE links(
	id SERIAL PRIMARY KEY,
	chain_id INTEGER REFERENCES chains(id),
	link_ordinal INTEGER NOT NULL,
	up_filename TEXT NULL,
	up_file_hash TEXT NULL,
	down_filename TEXT NULL,
	down_file_hash TEXT NULL,
	created_at TIMESTAMP NOT NULL,
	updated_at TIMESTAMP,

	CONSTRAINT chain_id_fkey FOREIGN KEY (chain_id)
		REFERENCES chains(id) MATCH SIMPLE
		ON UPDATE CASCADE
		ON DELETE CASCADE
);


INSERT INTO chains ( name, description, admin_email, created_at ) VALUES ( 'mutable', 'Mutable''s internal chain.', 'YOUR_EMAIL_HERE', NOW() );

