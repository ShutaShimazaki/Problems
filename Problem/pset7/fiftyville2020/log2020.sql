-- -- Keep a log of any SQL queries you execute as you solve the mystery.
-- --捜査記録--

-- SELECT description FROM crime_scene_reports
-- WHERE year= 2020 and month = 7 and day = 28;

-- --目撃者３人からのインタビュー--
-- SELECT * from interviews
-- WHERE year= 2020 and month = 7 and day = 28;

-- --ATMからお金を引き出した犯人--
-- SELECT * FROM atm_transactions
-- WHERE atm_location like "%Fifer street%" and year= 2020 and month = 7 and day = 28 and transaction_type = "withdraw";

-- --犯人は共犯者に電話をした--
-- SELECT * FROM phone_calls
-- WHERE year= 2020 and month = 7 and day = 28 and duration < 60;

-- --犯人は犯行時駐車場にあった車に乗り、走り去った--
-- SELECT * FROM courthouse_security_logs
-- WHERE year= 2020 and month = 7 and day = 28 and activity ="exit" and hour = 10;

-- --共犯者は飛行機のチケットを予約した--
-- SELECT * FROM flights
-- JOIN airports ON flights.origin_airport_id = airports.id
-- WHERE year= 2020 and month = 7 and day = 29 and airports.city = "Fiftyville"
-- ORDER BY hour ASC;

-- --destination_airport_id＝4とはどこ？--
-- SELECT * FROM airports
-- WHERE id = 4;

-- --共犯者が手配したとされるフライトid = 36内の乗客--
-- SELECT * FROM passengers
-- WHERE flight_id = 36;

-- --passport番号,  caller番号, 　が一致している人が犯人--
-- SELECT * FROM people
-- WHERE phone_number =(
-- SELECT phone_number FROM people
-- JOIN passengers ON people.passport_number = passengers.passport_number
-- JOIN flights ON flights.id = passengers.flight_id
-- WHERE flights.id = 36

-- INTERSECT

-- SELECT caller FROM phone_calls
-- WHERE year= 2020 and month = 7 and day = 28 and duration < 60);

-- -- --共犯者(996) 555-8899 --
-- -- SELECT * FROM people
-- -- WHERE phone_number = "(996) 555-8899";

-- --犯人の確かめ, atm番号--
-- -- SELECT * FROM atm_transactions
-- -- JOIN people ON people.id = bank_accounts.person_id
-- -- JOIN bank_accounts ON bank_accounts.account_number = atm_transactions.account_number
-- -- WHERE people.id = 398010;

-- -- SELECT * FROM bank_accounts
-- -- WHERE person_id = 567218;



-- --atmのやりとり、フライト番号、電話番号callerの情報に共通の人物--
-- --あとはナンバープレートの情報を加えるだけ＋最初の操作のミスの原因も
-- SELECT name, phone_number, people.passport_number, license_plate FROM people
-- JOIN bank_accounts ON people.id = bank_accounts.person_id
-- JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
-- WHERE atm_transactions.account_number in(
-- SELECT account_number FROM atm_transactions
-- WHERE atm_location like "%Fifer street%" and year= 2020 and month = 7 and day = 28 and transaction_type = "withdraw")

-- INTERSECT
-- SELECT name, phone_number, people.passport_number, license_plate FROM people
-- JOIN passengers ON people.passport_number = passengers.passport_number
-- WHERE passengers.flight_id = 36

-- INTERSECT

-- SELECT name, phone_number, people.passport_number, license_plate FROM people
-- JOIN phone_calls ON phone_calls.caller = people.phone_number
-- WHERE phone_calls.year= 2020 and phone_calls.month = 7 and phone_calls.day = 28 and phone_calls.duration < 60

-- INTERSECT
-- SELECT name, phone_number, people.passport_number, people.license_plate FROM people
-- JOIN courthouse_security_logs ON courthouse_security_logs.license_plate = people.license_plate
-- WHERE year= 2020 and month = 7 and day = 28 and activity ="exit" and hour = 10 and minute>15 and minute<25;

-- --犯人の全ての情報抽出ーー
-- SELECT * FROM people
-- WHERE name = "Ernest";

-- --電話番号の情報から共犯者決定ー
-- SELECT * FROM people
-- WHERE phone_number = (
-- SELECT receiver FROM phone_calls
-- WHERE phone_calls.caller = "(367) 555-5533"
-- and
-- phone_calls.year= 2020 and phone_calls.month = 7 and phone_calls.day = 28 and phone_calls.duration < 60);

--~~~~~2022!!~~~~--
SELECT * from interviews
WHERE year= 2021 and month = 7 and day = 28;
