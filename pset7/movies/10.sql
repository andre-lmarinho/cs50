SELECT DISTINCT p.name
FROM DIRECTORS AS d
    JOIN ratings AS r ON d.movie_id = r.movie_id
    JOIN people AS p ON d.person_id = p.id
WHERE r.rating >= 9.0;
