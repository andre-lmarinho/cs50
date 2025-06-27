import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.context_processor
def inject_username():
    """Make {{ username }} available for templates."""
    if session.get("user_id"):
        rows = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
        return {"username": rows[0]["username"]}
    return {}


@app.route("/")
@login_required
def index():
    """
    Show portfolio of stocks and cash balance.
    """
    user_id = session["user_id"]

    # Get user's cash balance
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = rows[0]["cash"]

    # Aggregate shares per symbol (only positive balances)
    positions = db.execute(
        """
        SELECT symbol, SUM(shares) AS shares
          FROM transactions
         WHERE user_id = ?
         GROUP BY symbol
        HAVING shares > 0
        """,
        user_id
    )

    # Build list of holdings with current price and total value
    holdings = []
    total_value = 0
    for pos in positions:
        quote = lookup(pos["symbol"])
        price = quote["price"]
        total = pos["shares"] * price
        total_value += total

        holdings.append({
            "name":   quote["name"],
            "symbol": quote["symbol"],
            "shares": pos["shares"],
            "price":  price,
            "total":  total
        })

    # Grand total = cash + all positions' value
    grand_total = cash + total_value

    return render_template(
        "index.html",
        holdings=holdings,
        cash=cash,
        grand_total=grand_total
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """
    Enable user to buy shares.
    GET:  Render form to input symbol and number of shares.
    POST: Validate input, check cash, record transaction, update cash, then redirect to home.
    """
    if request.method == "POST":
        # get form inputs
        symbol = request.form.get("symbol")
        shares_str = request.form.get("shares")

        # validate symbol
        if not symbol:
            return apology("must provide symbol", 400)
        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)

        # validate shares: positive integer
        if not shares_str or not shares_str.isdigit() or int(shares_str) < 1:
            return apology("must provide positive integer shares", 400)
        shares = int(shares_str)

        # calculate total cost
        price = quote["price"]
        cost = shares * price

        # check user cash
        row = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = row[0]["cash"]
        if cost > cash:
            return apology("can't afford", 400)

        # deduct cash
        db.execute(
            "UPDATE users SET cash = cash - ? WHERE id = ?",
            cost,
            session["user_id"]
        )

        # record transaction
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
            session["user_id"],
            quote["symbol"],
            shares,
            price
        )

        # redirect back to portfolio
        return redirect("/")

    # GET: show buy form
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """
    Show history of all transactions (buys and sells).
    """
    user_id = session["user_id"]

    # Query all transactions for this user, most recent first
    rows = db.execute(
        """
        SELECT symbol, shares, price, transacted
          FROM transactions
         WHERE user_id = ?
         ORDER BY transacted DESC
        """,
        user_id
    )

    # Fetch cash movements from ledger
    cash_movs = db.execute(
        """
        SELECT type, amount, balance_after, description, created_at AS datetime
          FROM ledger
         WHERE user_id = ?
         ORDER BY created_at DESC
        """,
        user_id
    )

    # Pass infos directly to template
    return render_template("history.html", transactions=rows, cash_movements=cash_movs)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """
    Look up a stock’s current price.
    GET:  Render form to input symbol.
    POST: Validate symbol, call lookup(), then render result.
    """
    if request.method == "POST":
        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Look up stock
        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)

        # Render a template showing name, symbol and price
        return render_template("quoted.html", name=quote["name"],
                               symbol=quote["symbol"],
                               price=usd(quote["price"]))

    # If GET, render form to ask for a symbol
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        # 1. Grab all form inputs right away
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        cash_str = request.form.get("starting_cash")

        # 2. Validate username/password
        if not username:
            return apology("must provide username", 400)
        if not password or not confirmation:
            return apology("must provide password and confirmation", 400)
        if password != confirmation:
            return apology("passwords do not match", 400)

        # 3. Parse starting cash (default 0)
        if cash_str:
            try:
                starting_cash = float(cash_str)
            except ValueError:
                return apology("starting cash must be numeric", 400)
            if starting_cash < 0:
                return apology("starting cash must be non-negative", 400)
        else:
            starting_cash = 0.0

        # 4. Hash password and insert user
        hash_pw = generate_password_hash(password)
        try:
            new_id = db.execute(
                "INSERT INTO users (username, hash, cash) VALUES (?, ?, ?)",
                username, hash_pw, starting_cash
            )
        except ValueError:
            return apology("username already exists", 400)

        # 5. Log in and record initial deposit
        session["user_id"] = new_id
        if starting_cash > 0:
            db.execute(
                """
                INSERT INTO ledger
                    (user_id, type, amount, balance_after, description)
                VALUES (?, 'deposit', ?, ?, 'Initial deposit')
                """,
                new_id,
                starting_cash,
                starting_cash
            )

        return redirect("/")

    # If GET, just show the form
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """
    Enable user to sell shares they own.
    GET:  Render form to select symbol and input number of shares.
    POST: Validate input, check holdings, record sale, update cash, then redirect to home.
    """
    user_id = session["user_id"]

    if request.method == "POST":
        # get form inputs
        symbol = request.form.get("symbol")
        shares_str = request.form.get("shares")

        # validate symbol selection
        if not symbol:
            return apology("must select symbol", 400)

        # validate shares: positive integer
        if not shares_str or not shares_str.isdigit() or int(shares_str) < 1:
            return apology("must provide positive integer shares", 400)
        shares = int(shares_str)

        # check user's current holdings of that symbol
        rows = db.execute(
            "SELECT SUM(shares) AS total FROM transactions WHERE user_id = ? AND symbol = ?",
            user_id,
            symbol
        )
        total_owned = rows[0]["total"] or 0
        if shares > total_owned:
            return apology("too many shares", 400)

        # lookup current price
        quote = lookup(symbol)
        price = quote["price"]
        revenue = shares * price

        # update user's cash
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?",
            revenue,
            user_id
        )

        # record the sale as negative shares
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
            user_id,
            symbol,
            -shares,
            price
        )

        # redirect back to portfolio
        return redirect("/")

    # GET: fetch symbols user owns to populate select menu
    positions = db.execute(
        """
        SELECT symbol, SUM(shares) AS shares
          FROM transactions
         WHERE user_id = ?
         GROUP BY symbol
        HAVING shares > 0
        """,
        user_id
    )

    return render_template("sell.html", positions=positions)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """
    Let a logged-in user change their password.
    GET:  Render form to input current and new passwords.
    POST: Validate inputs, check current password, update hash in DB, then redirect.
    """
    if request.method == "POST":
        # get form inputs
        current = request.form.get("current_password")
        newpw = request.form.get("new_password")
        confirm = request.form.get("confirmation")

        # ensure all fields submitted
        if not current or not newpw or not confirm:
            return apology("must fill all fields", 400)

        # ensure new passwords match
        if newpw != confirm:
            return apology("new passwords do not match", 400)

        # fetch user's current hash
        row = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])
        if not row:
            return apology("user not found", 400)
        current_hash = row[0]["hash"]

        # verify current password
        if not check_password_hash(current_hash, current):
            return apology("current password incorrect", 400)

        # everything ok → update to new hash
        new_hash = generate_password_hash(newpw)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", new_hash, session["user_id"])

        # optional: flash a success message
        flash("Password changed successfully")

        return redirect("/")

    # GET: render change-password form
    return render_template("change_password.html")


@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """
    Handle deposits and withdrawals.
    GET:  Show form to deposit/withdraw cash.
    POST: Validate input, update users.cash, record in ledger, then redirect.
    """
    user_id = session["user_id"]

    if request.method == "POST":
        # get form inputs
        action = request.form.get("action")       # 'deposit' or 'withdrawal'
        amount_str = request.form.get("amount")

        # validate action
        if action not in ("deposit", "withdrawal"):
            return apology("invalid action", 400)

        # validate amount
        if not amount_str:
            return apology("must provide amount", 400)
        try:
            amount = float(amount_str)
        except ValueError:
            return apology("amount must be numeric", 400)
        if amount <= 0:
            return apology("amount must be positive", 400)

        # determine signed amount
        signed_amount = amount if action == "deposit" else -amount

        # fetch current cash
        row = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = row[0]["cash"]

        # for withdrawal, ensure sufficient funds
        if signed_amount < 0 and abs(signed_amount) > cash:
            return apology("insufficient funds", 400)

        # compute new balance
        new_balance = cash + signed_amount

        # update user's cash
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            new_balance,
            user_id
        )

        # record movement in ledger
        db.execute(
            """
            INSERT INTO ledger
                (user_id, type, amount, balance_after, description)
            VALUES (?, ?, ?, ?, ?)
            """,
            user_id,
            action,
            signed_amount,
            new_balance,
            action.capitalize()
        )

        flash(f"{action.capitalize()} of {usd(amount)} successful")
        return redirect("/")

    # GET: render the cash form
    return render_template("cash.html")
