SELECT title FROM movies
JOIN people ON people.id = stars.person_id
JOIN stars ON stars.movie_id = movies.id
JOIN ratings ON movies.id = ratings.movie_id
WHERE people.name = "Chadwick Boseman"
ORDER BY ratings.rating DESC
LIMIT 5;