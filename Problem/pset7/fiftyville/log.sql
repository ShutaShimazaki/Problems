-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description FROM crime_scene_reports
WHERE year= 2021 and month = 7 and day = 28;

SELECT * from interviews
WHERE year= 2021 and month = 7 and day = 28;

--犯人探し--
SELECT * FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE

atm_transactions.account_number in(
SELECT account_number FROM atm_transactions
WHERE atm_location = "Leggett Street" and year= 2021 and month = 7 and atm_transactions.day = 28 and transaction_type = "withdraw")

and

people.passport_number in(
SELECT people.passport_number FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE passengers.flight_id =(
SELECT flights.id FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year= 2021 and month = 7 and day = 29 and airports.city = "Fiftyville"
ORDER BY hour ASC
LIMIT 1))

and

people.phone_number in (
SELECT people.phone_number FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE phone_calls.year= 2021 and phone_calls.month = 7 and phone_calls.day = 28 and phone_calls.duration < 60)

and

people.license_plate in (
SELECT people.license_plate FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE year= 2021 and month = 7 and day = 28 and activity ="exit" and hour = 10 and minute>15 and minute<25);

-- and

-- people.license_plate in (
-- SELECT license_plate FROM bakery_security_logs
-- JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
-- WHERE year= 2021 and month = 7 and day = 28 and activity ="exit" and hour = 10 and minute>15 and minute<25);


---共犯者探し--
SELECT * FROM people
WHERE phone_number = (
SELECT receiver FROM phone_calls
WHERE phone_calls.caller = "(367) 555-5533"
and
phone_calls.year= 2021 and phone_calls.month = 7 and phone_calls.day = 28 and phone_calls.duration < 60);

--逃げた先は・・・--
SELECT city FROM airports
JOIN flights ON flights.destination_airport_id = airports.id
JOIN passengers ON passengers.flight_id = flights.id
WHERE
passengers.passport_number = 1988161715
and
flights.year = 2021 and flights.month = 7 and flights.day = 29;


--確かめ
SELECT * FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE year= 2021 and month = 7 and day = 29 and airports.city = "Fiftyville"
ORDER BY hour ASC;

SELECT * FROM airports
WHERE id = 4;

SELECT * FROM passengers
WHERE flight_id = 36;

SELECT * FROM phone_calls
WHERE year= 2021 and month = 7 and day = 28;