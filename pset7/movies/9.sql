SELECT DISTINCT p.name
FROM people AS p
    JOIN stars AS s ON p.id = s.person_id
    JOIN movies AS m ON s.movie_id = m.id
WHERE m.year = 2004 AND p.birth IS NOT NULL ORDER BY p.birth ASC;
