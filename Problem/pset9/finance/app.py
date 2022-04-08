# export API_KEY=pk_442e04595e474291b14a359d1f4ad9b8
# zaki shimashima

import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # # lookup関数は、とあるsymbolについて「symbol」「formalname」「price」を与えてくれる関数

    # 今ログインしているユーザーのuser_idを取得
    user_id = session["user_id"]


# データベースから、今ログインしているユーザーの取引の[symbol]列のみを抽出
# 結果： [{'symbol': 'Apple Inc'}, {'symbol': 'Apple Inc'}, {'symbol': 'AMZN'}] ]のようになった
# 最終的には、1つのsymbolについての取引は合わせる。合わせたのちにhtmlへ。それを各symbolについて行いたい

    # list = db.execute(
    #     "SELECT symbol, formalname, SUM(shares), stockprice FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)

    onlysymbols = db.execute(
        "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    # onlysymbols = [{'symbol': 'AAPL'}]

    symbols = db.execute(
        "SELECT symbol, formalname, SUM(shares), stockprice FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)

    stockprice_dict = {}
    for symbol in onlysymbols:
        stockprice_dict[symbol["symbol"]] = (lookup(symbol["symbol"])["price"])
    # stockprice_dict =  {'AAPL': 176.28, 'NFLX': 412.89}

    # for symbol in symbols:
    #     total = symbol["stockprice"] * symbol["SUM(shares)"]

    # cashを定義して、index.htmlに渡す
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[
        0]["cash"]

    # total =　（現在の株価　* 株数）　＋ cash

    total = cash
    for symbol in symbols:
        total += (stockprice_dict[symbol["symbol"]] * symbol["SUM(shares)"])

    return render_template("index.html", symbols=symbols, stockprice_dict=stockprice_dict, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # ユーザー入力の不備について
        if not request.form.get("symbol"):
            return apology("Enter a name of symbol, please", 400)

        if lookup(request.form.get("symbol")) == None:
            return apology("Can't find the symbol", 400)

        if not request.form.get("shares"):
            return apology("Enter how many shares you wanna get, please", 400)

        def is_integer(n):
            try:
                float(n)
            except ValueError:
                return False
            else:
                return float(n).is_integer()

        if (is_integer(request.form.get("shares"))) == False or float(request.form.get("shares")) <= 0:
            return apology("Shares must be a positive integer", 400)

        # 入力に不備がない場合
        # lookupを呼び出して株の現在の価格を調べる
        if lookup(request.form.get("symbol")) != None:

            # login関数において、session["user_id"] = rows[0]["id"]とした。
            user_id = session["user_id"]

            stockprice = lookup(request.form.get("symbol"))["price"]
            shares = int(request.form.get("shares"))
            symbol = lookup(request.form.get("symbol"))["symbol"]

            formalname = lookup(request.form.get("symbol"))["name"]

            # ユーザが現在の価格で株式数を購入できるかチェック
            # stockprice * shares > cash ならば購入不可能だからapology
            cash_before_buy = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[
                0]["cash"]
            cash_after_buy = (cash_before_buy) - (shares * stockprice)
            time = db.execute(
                "SELECT datetime('now','+9 hours')")[0]["datetime('now','+9 hours')"]

            print(f"\n\n {time}\n\n")

            if (stockprice * shares > cash_before_buy):
                return apology("Luck of Money", 400)

            db.execute("INSERT INTO transactions (user_id,stockprice,shares, symbol, type, time, formalname) VALUES (?, ?, ?, ?, ?, ?, ?)",
                       user_id, stockprice, shares, symbol, "test", time, formalname)

            # データベース上の株購入後の現金cash情報を更新
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       cash_after_buy, user_id)
        return redirect("/")

    # リクエストがGETならば
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    history = db.execute(
        "SELECT symbol, shares , stockprice, time FROM transactions WHERE user_id = ?", user_id)
    # print(f"\n\nhistory  {history}\n\n")
    # history =[{'symbol': 'AAPL', 'shares': 5, 'stockprice': 176.28, 'time': '2022-02-10 20:59:00'}, {'symbol': 'NFLX', 'shares': 1, 'stockprice': 412.89, 'time': '2022-02-10 21:17:16'}, {'symbol': 'AAPL', 'shares': -4, 'stockprice': 175.035, 'time': '2022-02-11 00:08:53'}]

    charge_history = db.execute(
        "SELECT * FROM charge WHERE user_id = ?", user_id)

    return render_template("history.html", history=history, charge_history=charge_history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


# POSTでsymbol名が送られてきたら
# 会社の株価の情報を得る
# →quoted.htmlに表示

# GETならば(else)、quote.htmlを表示
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

# quote.htmlのフォームにて問い合わせされた会社名："symbol"
# lookup(request.form.get("symbol"))　の戻り値が辞書になるはず
    if request.method == "POST":
        if lookup(request.form.get("symbol")) != None:
            stockprice = lookup(request.form.get("symbol"))["price"]
            symbol = lookup(request.form.get("symbol"))["name"]
            return render_template("quoted.html", stockprice=stockprice, symbol=symbol)

        else:
            return apology("Can't find the symbol", 400)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # GETリクエストなら、登録画面を表示する
    if request.method == "GET":
        return render_template("register.html")

    # POSTリクエストなら、usernameやpasswordの情報が送られているということ。

    # データベースにusernameとpasswordを追加する
    if request.method == "POST":

        for i in db.execute("SELECT username FROM users"):
            if request.form.get("username") == i["username"]:
                return apology("This username is already used.", 400)

        # まずusernameとpasswordが入力されているか確かめる。
        if not request.form.get("username"):
            return apology("Must enter username", 400)

        elif not request.form.get("password"):
            return apology("Must enter password", 400)

        elif not request.form.get("confirmation"):
            return apology("Must enter password(again)", 400)

        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("First password doesn't match the second one", 400)

        else:

            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                       request.form.get("username"), generate_password_hash(request.form.get("password"), method='sha256'))

            # db.execute("INSERT INTO users (hash) VALUES (?)",
            #            generate_password_hash(request.form.get("password"), method='sha256'))

            return redirect("/login")


@ app.route("/sell", methods=["GET", "POST"])
@ login_required
def sell():
    """Sell shares of stock"""

# 所持しているsymbol名とSUM(shares)を取得
    user_id = session["user_id"]

    symbols = db.execute(
        "SELECT symbol, SUM(shares) FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    print(f"\n\nsymbols =  {symbols} \n\n")
    # symbols =  [{'symbol': 'AAPL', 'SUM(shares)': 5}, {'symbol': 'NFLX', 'SUM(shares)': 1}]

    if request.method == "POST":

        # ユーザーが①株式を選択しない　②sharesの入力が正の整数ではない　③保有数以上の株式数を購入した
        # 上記の場合は apology
        if not request.form.get("symbol"):
            return apology("Select a symbol", 400)

        if not request.form.get("shares"):
            return apology("Enter shares", 400)

        if int(request.form.get("shares")) <= 0:
            return apology("Enter positive integer", 400)

        if int(request.form.get("shares")) > symbols[0]["SUM(shares)"]:
            return apology("You don't have the amount of shares!", 400)

# やることは2つ
# 1. transactions　テーブルに売った取引を記録
# 2.　index.htmlにおいてtotalは、usersテーブルのcashから情報を得ている
# 情報を更新するためにcash情報を更新→index.htmlに反映させる
        requ_symbol = request.form.get("symbol")
        print(f"\n\nrequ_get_symbol =  {requ_symbol} \n\n")

        stockprice = lookup(request.form.get("symbol"))["price"]
        shares = int(request.form.get("shares"))
        symbol = lookup(request.form.get("symbol"))["symbol"]
        formalname = lookup(request.form.get("symbol"))["name"]
        time = db.execute(
            "SELECT datetime('now','+9 hours')")[0]["datetime('now','+9 hours')"]

# 1. transactions　テーブルに売った取引を記録
        db.execute("INSERT INTO transactions (user_id,stockprice,shares, symbol, type, time, formalname) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   user_id, stockprice, -(shares), symbol, "sell", time, formalname)


# 2.　usersテーブルのcash情報を更新→index.htmlに反映させる
        cash_before_sell = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[
            0]["cash"]
        cash_after_sell = (cash_before_sell) + (shares * stockprice)

        # データベース上の株購入後の現金cash情報を更新
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   cash_after_sell, user_id)

        return redirect("/")

    else:
        return render_template("sell.html", symbols=symbols)


@ app.route("/charge", methods=["GET", "POST"])
@ login_required
def charge():
    if request.method == "POST":
        user_id = session["user_id"]
        money = request.form.get("money")

        if not request.form.get("money"):
            return apology("Enter the amount of charge", 400)

        # userテーブルのcashを+(money)へと更新する
        cash_before_charge = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[
            0]["cash"]
        cash_after_charge = (cash_before_charge) + int(money)

        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   cash_after_charge, user_id)

        # chargeテーブルにチャージ履歴を残す。history.htmlで確認できるようにする
        time = db.execute(
            "SELECT datetime('now','+9 hours')")[0]["datetime('now','+9 hours')"]

        db.execute("INSERT INTO charge (user_id,time, amount, cash_now) VALUES (?, ?, ?, ?)",
                   user_id, time, money, cash_after_charge)

        # これはhistory()へ移動
        # charge_history = db.execute(
        #     "SELECT * FROM charge WHERE user_id = ?", user_id)

        return redirect("/")

    else:
        return render_template("charge.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
