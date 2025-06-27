SELECT DISTINCT m.title
FROM movies AS m
    JOIN stars AS s ON m.id = s.movie_id
    JOIN people AS p ON p.id = s.person_id
    JOIN ratings AS r ON r.movie_id = m.id
WHERE p.name ='Chadwick Boseman'
ORDER BY r.rating DESC, m.title ASC
LIMIT 5;
