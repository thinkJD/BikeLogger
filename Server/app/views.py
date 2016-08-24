from flask import render_template, flash, redirect, session, url_for, request, g
from flask_login import login_user, logout_user, current_user, login_required
from app import app, db, lm
from .forms import LoginForm, SyncForm
from .models import User


@app.route('/')
@app.route('/index')
@login_required
def index():
    user = g.user
    posts = [
        {
            'author': {'nickname': 'John'},
            'body': 'Beautiful day in Portland!'
        },
        {
            'author': {'nickname': 'Susan'},
            'body': 'The Avengers movie was so cool!'
        }
    ]
    return render_template('index.html',
                           title='Home',
                           user=user,
                           posts=posts)


@app.route('/login', methods=['GET', 'POST'])
def login():
    # user is already logged in
    if g.user is not None and g.user.is_authenticated:
        return redirect(url_for('index'))

    # if not, render login view
    login_form = LoginForm()
    if login_form.validate_on_submit():
        session['remember_me'] = login_form.remember_me.data
        usr = User.query.filter_by(email=login_form.email.data).first()
        if usr:
            login_user(usr, True)
            return redirect(url_for('index'))
        else:
            flash('User not found')

    return render_template('login.html',
                           title='Sign In',
                           form=login_form)

@app.route('/sync', methods=['GET', 'POST'])
@login_required
def sync():
    # todo check user login
    sync_form = SyncForm()
    if sync_form.validate_on_submit():
        print('Start Sync!')
    return render_template('sync.html',
                           title='Sync',
                           form=sync_form)

@app.route('/logout')
def logout():
    logout_user()
    return redirect(url_for('index'))

@app.before_request
def before_request():
    g.user = current_user

@lm.user_loader
def load_user(id):
    return User.query.get(int(id))



