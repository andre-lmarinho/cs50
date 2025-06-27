SELECT p.name FROM people AS p
JOIN stars AS s ON p.id = s.person_id
WHERE s.movie_id = (
    SELECT id FROM movies WHERE title = 'Toy Story'
);
