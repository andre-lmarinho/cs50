SELECT DISTINCT p.name
FROM stars AS s
    JOIN people AS p ON s.person_id = p.id
WHERE s.movie_id IN (
    SELECT movie_id FROM stars WHERE person_id =(
        SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958
    )
)
AND p.name <> 'Kevin Bacon';
