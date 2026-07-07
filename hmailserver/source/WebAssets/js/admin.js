(function() {
    'use strict';

    var Admin = {
        token: null,
        currentPage: 'dashboard',

        init: async function() {
            this.token = localStorage.getItem('admin_token');

            if (!this.token) {
                this.showLogin();
                return;
            }

            var valid = await this.checkAuth();
            if (!valid) {
                this.showLogin();
                return;
            }

            this.renderShell();
            this.navigate(window.location.hash.slice(1) || 'dashboard');
        },

        checkAuth: async function() {
            try {
                var res = await fetch('/api/auth/check', {
                    headers: { 'Authorization': 'Bearer ' + this.token }
                });
                return res.ok;
            } catch (e) {
                return false;
            }
        },

        showLogin: function() {
            var modal = document.getElementById('modal');
            modal.style.display = 'flex';
            modal.innerHTML =
                '<div class="modal-content">' +
                    '<h2 class="modal-title">' + Lang.get('admin.login.title') + '</h2>' +
                    '<div class="form-group">' +
                        '<label>' + Lang.get('admin.login.email') + '</label>' +
                        '<input type="email" id="login-email">' +
                    '</div>' +
                    '<div class="form-group">' +
                        '<label>' + Lang.get('admin.login.password') + '</label>' +
                        '<input type="password" id="login-password">' +
                    '</div>' +
                    '<div class="buttons">' +
                        '<button class="btn primary" id="login-btn">' + Lang.get('admin.login.submit') + '</button>' +
                    '</div>' +
                    '<div id="login-error" style="color:#ff6b6b;margin-top:1rem;"></div>' +
                '</div>';

            document.getElementById('login-btn').addEventListener('click', this.doLogin.bind(this));
            document.getElementById('login-password').addEventListener('keypress', function(e) {
                if (e.key === 'Enter') Admin.doLogin();
            });
        },

        doLogin: async function() {
            var email = document.getElementById('login-email').value;
            var password = document.getElementById('login-password').value;
            var errorEl = document.getElementById('login-error');

            try {
                var res = await fetch('/api/auth/login', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ email: email, password: password })
                });

                var data = await res.json();

                if (data.success) {
                    this.token = data.data.token;
                    localStorage.setItem('admin_token', this.token);
                    document.getElementById('modal').style.display = 'none';
                    this.renderShell();
                    this.navigate('dashboard');
                } else {
                    errorEl.textContent = data.error || Lang.get('admin.login.error');
                }
            } catch (e) {
                errorEl.textContent = Lang.get('common.error');
            }
        },

        logout: function() {
            fetch('/api/auth/logout', {
                method: 'POST',
                headers: { 'Authorization': 'Bearer ' + this.token }
            });
            localStorage.removeItem('admin_token');
            this.token = null;
            window.location.reload();
        },

        renderShell: function() {
            var sidebar = document.getElementById('sidebar');
            sidebar.innerHTML =
                '<div class="sidebar-header"><h1>Mailaholic</h1></div>' +
                '<a href="#dashboard" class="nav-item" data-page="dashboard">' + Lang.get('admin.nav.dashboard') + '</a>' +
                '<a href="#domains" class="nav-item" data-page="domains">' + Lang.get('admin.nav.domains') + '</a>';

            var topbar = document.getElementById('topbar');
            topbar.innerHTML =
                '<span class="topbar-title" id="page-title"></span>' +
                '<div class="topbar-actions">' +
                    '<button class="btn secondary" id="logout-btn">' + Lang.get('admin.logout') + '</button>' +
                '</div>';

            document.getElementById('logout-btn').addEventListener('click', this.logout.bind(this));

            var self = this;
            window.addEventListener('hashchange', function() {
                self.navigate(window.location.hash.slice(1) || 'dashboard');
            });
        },

        navigate: function(page) {
            this.currentPage = page;

            document.querySelectorAll('.nav-item').forEach(function(item) {
                item.classList.toggle('active', item.dataset.page === page);
            });

            switch (page) {
                case 'dashboard':
                    this.loadDashboard();
                    break;
                case 'domains':
                    this.loadDomains();
                    break;
                default:
                    if (page.startsWith('accounts/')) {
                        this.loadAccounts(page.split('/')[1]);
                    } else {
                        this.loadDashboard();
                    }
            }
        },

        api: async function(endpoint, method, body) {
            method = method || 'GET';
            var opts = {
                method: method,
                headers: {
                    'Authorization': 'Bearer ' + this.token,
                    'Content-Type': 'application/json'
                }
            };
            if (body) opts.body = JSON.stringify(body);

            var res = await fetch(endpoint, opts);
            return await res.json();
        },

        loadDashboard: async function() {
            document.getElementById('page-title').textContent = Lang.get('admin.nav.dashboard');

            var content = document.getElementById('page-content');
            content.innerHTML = '<div class="loading">' + Lang.get('common.loading') + '</div>';

            var data = await this.api('/api/dashboard');

            if (data.success) {
                var d = data.data;
                content.innerHTML =
                    '<div class="stats-grid">' +
                        '<div class="stat-card">' +
                            '<div class="stat-value">' + d.domainCount + '</div>' +
                            '<div class="stat-label">' + Lang.get('admin.dashboard.domains') + '</div>' +
                        '</div>' +
                        '<div class="stat-card">' +
                            '<div class="stat-value">' + d.accountCount + '</div>' +
                            '<div class="stat-label">' + Lang.get('admin.dashboard.accounts') + '</div>' +
                        '</div>' +
                        '<div class="stat-card">' +
                            '<div class="stat-value"><span class="badge success">' + d.serverStatus + '</span></div>' +
                            '<div class="stat-label">' + Lang.get('admin.dashboard.status') + '</div>' +
                        '</div>' +
                    '</div>';
            }
        },

        loadDomains: async function() {
            var self = this;
            document.getElementById('page-title').textContent = Lang.get('admin.nav.domains');

            var content = document.getElementById('page-content');
            content.innerHTML = '<div class="loading">' + Lang.get('common.loading') + '</div>';

            var data = await this.api('/api/domains');

            if (data.success) {
                var domains = data.data;
                var html =
                    '<div class="card">' +
                        '<div class="card-header">' +
                            '<span class="card-title">' + Lang.get('admin.domains.title') + '</span>' +
                            '<button class="btn primary" id="add-domain-btn">' + Lang.get('admin.domains.add') + '</button>' +
                        '</div>' +
                        '<table class="data-table">' +
                            '<thead><tr>' +
                                '<th>' + Lang.get('admin.domains.name') + '</th>' +
                                '<th>' + Lang.get('admin.domains.status') + '</th>' +
                                '<th>' + Lang.get('admin.domains.actions') + '</th>' +
                            '</tr></thead>' +
                            '<tbody>';

                for (var i = 0; i < domains.length; i++) {
                    var d = domains[i];
                    html +=
                        '<tr>' +
                            '<td><a href="#accounts/' + d.id + '">' + d.name + '</a></td>' +
                            '<td><span class="badge ' + (d.isActive ? 'success' : 'warning') + '">' +
                                (d.isActive ? 'Active' : 'Inactive') + '</span></td>' +
                            '<td>' +
                                '<button class="action-btn danger" data-delete="' + d.id + '">' + Lang.get('admin.delete') + '</button>' +
                            '</td>' +
                        '</tr>';
                }

                html += '</tbody></table></div>';
                content.innerHTML = html;

                document.getElementById('add-domain-btn').addEventListener('click', function() {
                    self.showAddDomainModal();
                });

                content.querySelectorAll('[data-delete]').forEach(function(btn) {
                    btn.addEventListener('click', function() {
                        self.deleteDomain(btn.dataset.delete);
                    });
                });
            }
        },

        showAddDomainModal: function() {
            var self = this;
            var modal = document.getElementById('modal');
            modal.style.display = 'flex';
            modal.innerHTML =
                '<div class="modal-content">' +
                    '<h2 class="modal-title">' + Lang.get('admin.domains.add') + '</h2>' +
                    '<div class="form-group">' +
                        '<label>' + Lang.get('admin.domains.name') + '</label>' +
                        '<input type="text" id="domain-name" placeholder="example.com">' +
                    '</div>' +
                    '<div class="buttons">' +
                        '<button class="btn secondary" id="cancel-btn">' + Lang.get('admin.cancel') + '</button>' +
                        '<button class="btn primary" id="save-btn">' + Lang.get('admin.save') + '</button>' +
                    '</div>' +
                '</div>';

            document.getElementById('cancel-btn').addEventListener('click', function() {
                modal.style.display = 'none';
            });

            document.getElementById('save-btn').addEventListener('click', async function() {
                var name = document.getElementById('domain-name').value;
                if (name) {
                    await self.api('/api/domains', 'POST', { name: name });
                    modal.style.display = 'none';
                    self.loadDomains();
                }
            });
        },

        deleteDomain: async function(id) {
            if (confirm(Lang.get('admin.domains.confirmDelete'))) {
                await this.api('/api/domains/' + id, 'DELETE');
                this.loadDomains();
            }
        },

        loadAccounts: async function(domainId) {
            var self = this;
            document.getElementById('page-title').textContent = Lang.get('admin.nav.accounts');

            var content = document.getElementById('page-content');
            content.innerHTML = '<div class="loading">' + Lang.get('common.loading') + '</div>';

            var data = await this.api('/api/accounts?domain_id=' + domainId);

            if (data.success) {
                var accounts = data.data;
                var html =
                    '<div class="card">' +
                        '<div class="card-header">' +
                            '<span class="card-title">' + Lang.get('admin.accounts.title') + '</span>' +
                            '<button class="btn primary" id="add-account-btn">' + Lang.get('admin.accounts.add') + '</button>' +
                        '</div>' +
                        '<table class="data-table">' +
                            '<thead><tr>' +
                                '<th>' + Lang.get('admin.accounts.address') + '</th>' +
                                '<th>' + Lang.get('admin.accounts.status') + '</th>' +
                                '<th>' + Lang.get('admin.accounts.actions') + '</th>' +
                            '</tr></thead>' +
                            '<tbody>';

                for (var i = 0; i < accounts.length; i++) {
                    var a = accounts[i];
                    html +=
                        '<tr>' +
                            '<td>' + a.address + '</td>' +
                            '<td><span class="badge ' + (a.isActive ? 'success' : 'warning') + '">' +
                                (a.isActive ? 'Active' : 'Inactive') + '</span></td>' +
                            '<td>' +
                                '<button class="action-btn danger" data-delete="' + a.id + '">' + Lang.get('admin.delete') + '</button>' +
                            '</td>' +
                        '</tr>';
                }

                html += '</tbody></table></div>';
                content.innerHTML = html;

                document.getElementById('add-account-btn').addEventListener('click', function() {
                    self.showAddAccountModal(domainId);
                });

                content.querySelectorAll('[data-delete]').forEach(function(btn) {
                    btn.addEventListener('click', function() {
                        self.deleteAccount(btn.dataset.delete, domainId);
                    });
                });
            }
        },

        showAddAccountModal: function(domainId) {
            var self = this;
            var modal = document.getElementById('modal');
            modal.style.display = 'flex';
            modal.innerHTML =
                '<div class="modal-content">' +
                    '<h2 class="modal-title">' + Lang.get('admin.accounts.add') + '</h2>' +
                    '<div class="form-group">' +
                        '<label>' + Lang.get('admin.accounts.address') + '</label>' +
                        '<input type="email" id="account-address" placeholder="user@example.com">' +
                    '</div>' +
                    '<div class="form-group">' +
                        '<label>' + Lang.get('admin.accounts.password') + '</label>' +
                        '<input type="password" id="account-password">' +
                    '</div>' +
                    '<div class="buttons">' +
                        '<button class="btn secondary" id="cancel-btn">' + Lang.get('admin.cancel') + '</button>' +
                        '<button class="btn primary" id="save-btn">' + Lang.get('admin.save') + '</button>' +
                    '</div>' +
                '</div>';

            document.getElementById('cancel-btn').addEventListener('click', function() {
                modal.style.display = 'none';
            });

            document.getElementById('save-btn').addEventListener('click', async function() {
                var address = document.getElementById('account-address').value;
                var password = document.getElementById('account-password').value;
                if (address && password) {
                    await self.api('/api/accounts', 'POST', {
                        domain_id: parseInt(domainId),
                        address: address,
                        password: password
                    });
                    modal.style.display = 'none';
                    self.loadAccounts(domainId);
                }
            });
        },

        deleteAccount: async function(id, domainId) {
            if (confirm(Lang.get('admin.accounts.confirmDelete'))) {
                await this.api('/api/accounts/' + id, 'DELETE');
                this.loadAccounts(domainId);
            }
        }
    };

    window.Admin = Admin;

    document.addEventListener('DOMContentLoaded', async function() {
        await App.init();
        Admin.init();
    });
})();
