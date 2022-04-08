SELECT distinct name FROM people
JOIN movies ON movies.id = stars.movie_id
JOIN stars ON stars.person_id = people.id
WHERE name!="Kevin Bacon" and  stars.movie_id IN(
/*以下でKevinの出演映画全てを抽出
出演映画は１つではなく複数あるはず。
その複数の映画のid全てを上のスクリプトで利用したいから[IN]を使う*/
SELECT movies.id FROM movies
JOIN people ON people.id = stars.person_id
JOIN stars ON stars.movie_id = movies.id
WHERE people.name = "Kevin Bacon" AND people.birth = 1958);