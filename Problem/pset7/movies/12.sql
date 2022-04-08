/*
SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Johnny Depp" AND people.name ="Helena Bonham Carter";

「people.name」には一人の名前しか入っていないのだから、people.name = "Johnny Depp" AND people.name ="Helena Bonham Carter"はだめ。

Johnnyの映画　かつ(INTERSECT)　Helna の映画　にする
*/
SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name ="Johnny Depp"
INTERSECT
SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name ="Helena Bonham Carter";