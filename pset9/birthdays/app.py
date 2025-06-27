import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    #Get ID, if on querry string
    edit_id = request.args.get("id")

    if request.method == "POST":
        # Get form data (New or Update)
        id     = request.form.get("id")
        name   = request.form.get("name")
        month  = request.form.get("month")
        day    = request.form.get("day")

        # Name Validation Presence
        if not name or not month or not day:
                error = "Missing name, month or day."
                rows = db.execute("SELECT * FROM birthdays ORDER BY month, day")
                return render_template("index.html", birthdays=rows, error=error)

        # Dates Validation (Month and Day)
        try:
             month = int(month)
             day   = int(day)
        except ValueError:
             error = "Month and day must be numbers."
             rows  = db.execute("SELECT * FROM birthdays ORDER BY month, day")
             return render_template("index.html", birthdays=rows, error=error)

        if not (1 <= month <= 12) or not (1 <= day <= 31):
             error = "Use a valid date."
             rows  = db.execute("SELECT * FROM birthdays ORDER BY month, day")
             return render_template("index.html", birthdays=rows, error=error)

        # ID Validation If Update, Else Insert
        if id:
            db.execute(
                "UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?",
                name, month, day, id
            )
        else:
            db.execute(
                "INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)",
                name, month, day
            )

        return redirect("/")

    #GET: search ordered data
    rows = db.execute("SELECT * FROM birthdays ORDER BY month, day")
    return render_template("index.html", birthdays = rows, edit_id=edit_id, error=None)


@app.route("/delete", methods=["POST"])
def delete():
    # Get the id from the form and delete that row
    id = request.form.get("id")
    db.execute("DELETE FROM birthdays WHERE id = ?", id)
    return redirect("/")
