SELECT name FROM people
JOIN ratings ON ratings.movie_id = movies.id
JOIN movies ON movies.id = directors.movie_id
JOIN directors ON people.id = directors.person_id
WHERE ratings.rating >= 9.0;