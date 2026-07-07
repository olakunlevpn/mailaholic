(function() {
    'use strict';

    var Setup = {
        state: {},
        currentStep: 1,
        totalSteps: 6,

        init: async function() {
            await this.loadState();
            this.render();
        },

        loadState: async function() {
            try {
                var res = await fetch('/api/setup/state');
                var data = await res.json();
                if (data.success) {
                    this.state = data.data;
                    this.currentStep = this.state.step || 1;
                }
            } catch (e) {
                console.error('Failed to load setup state:', e);
            }
        },

        render: function() {
            this.updateProgress();
            var content = document.getElementById('wizard-content');
            content.innerHTML = this.getStepHtml();
            this.bindEvents();
        },

        updateProgress: function() {
            var progress = document.getElementById('progress');
            var pct = ((this.currentStep - 1) / (this.totalSteps - 1)) * 100;
            progress.style.width = pct + '%';

            var steps = document.getElementById('steps');
            steps.innerHTML = '';
            for (var i = 1; i <= this.totalSteps; i++) {
                var dot = document.createElement('div');
                dot.className = 'step-dot' + (i <= this.currentStep ? ' active' : '');
                steps.appendChild(dot);
            }
        },

        getStepHtml: function() {
            switch (this.currentStep) {
                case 1: return this.step1Welcome();
                case 2: return this.step2Usage();
                case 3: return this.step3Admin();
                case 4: return this.step4AutoConfig();
                case 5: return this.step5Dns();
                case 6: return this.step6Done();
                default: return '';
            }
        },

        step1Welcome: function() {
            return '<div class="step step-1">' +
                '<h2>' + Lang.get('setup.welcome.title') + '</h2>' +
                '<p class="subtitle">' + Lang.get('setup.welcome.subtitle') + '</p>' +
                '<div class="form-group">' +
                    '<label>' + Lang.get('setup.welcome.domain.label') + '</label>' +
                    '<input type="text" id="domain" placeholder="' + Lang.get('setup.welcome.domain.placeholder') + '" value="' + (this.state.domain || '') + '">' +
                    '<small>' + Lang.get('setup.welcome.domain.help') + '</small>' +
                '</div>' +
                '<div class="buttons">' +
                    '<button class="btn primary" id="next">' + Lang.get('setup.welcome.next') + '</button>' +
                '</div>' +
            '</div>';
        },

        step2Usage: function() {
            var usage = this.state.usageType || 0;
            return '<div class="step step-2">' +
                '<h2>' + Lang.get('setup.usage.title') + '</h2>' +
                '<div class="radio-group">' +
                    '<label class="radio-card ' + (usage === 0 ? 'selected' : '') + '">' +
                        '<input type="radio" name="usage" value="0" ' + (usage === 0 ? 'checked' : '') + '>' +
                        '<span class="title">' + Lang.get('setup.usage.personal') + '</span>' +
                        '<span class="desc">' + Lang.get('setup.usage.personal.desc') + '</span>' +
                    '</label>' +
                    '<label class="radio-card ' + (usage === 1 ? 'selected' : '') + '">' +
                        '<input type="radio" name="usage" value="1" ' + (usage === 1 ? 'checked' : '') + '>' +
                        '<span class="title">' + Lang.get('setup.usage.organization') + '</span>' +
                        '<span class="desc">' + Lang.get('setup.usage.organization.desc') + '</span>' +
                    '</label>' +
                    '<label class="radio-card ' + (usage === 2 ? 'selected' : '') + '">' +
                        '<input type="radio" name="usage" value="2" ' + (usage === 2 ? 'checked' : '') + '>' +
                        '<span class="title">' + Lang.get('setup.usage.highvolume') + '</span>' +
                        '<span class="desc">' + Lang.get('setup.usage.highvolume.desc') + '</span>' +
                    '</label>' +
                '</div>' +
                '<div class="buttons">' +
                    '<button class="btn secondary" id="back">' + Lang.get('setup.usage.back') + '</button>' +
                    '<button class="btn primary" id="next">' + Lang.get('setup.usage.next') + '</button>' +
                '</div>' +
            '</div>';
        },

        step3Admin: function() {
            return '<div class="step step-3">' +
                '<h2>' + Lang.get('setup.admin.title') + '</h2>' +
                '<div class="form-group">' +
                    '<label>' + Lang.get('setup.admin.email.label') + '</label>' +
                    '<input type="email" id="email" placeholder="' + Lang.get('setup.admin.email.placeholder') + '" value="' + (this.state.adminEmail || '') + '">' +
                '</div>' +
                '<div class="form-group">' +
                    '<label>' + Lang.get('setup.admin.password.label') + '</label>' +
                    '<input type="password" id="password" placeholder="' + Lang.get('setup.admin.password.placeholder') + '">' +
                '</div>' +
                '<div class="form-group">' +
                    '<label>' + Lang.get('setup.admin.confirm.label') + '</label>' +
                    '<input type="password" id="confirm">' +
                '</div>' +
                '<div class="buttons">' +
                    '<button class="btn secondary" id="back">' + Lang.get('setup.admin.back') + '</button>' +
                    '<button class="btn primary" id="next">' + Lang.get('setup.admin.next') + '</button>' +
                '</div>' +
            '</div>';
        },

        step4AutoConfig: function() {
            return '<div class="step step-4">' +
                '<h2>' + Lang.get('setup.autoconfig.title') + '</h2>' +
                '<div class="check-list" id="checks">' +
                    '<div class="check-item loading"><span class="icon">&#8987;</span> ' + Lang.get('common.loading') + '</div>' +
                '</div>' +
                '<div class="buttons">' +
                    '<button class="btn secondary" id="back">' + Lang.get('setup.autoconfig.back') + '</button>' +
                    '<button class="btn primary" id="next" disabled>' + Lang.get('setup.autoconfig.next') + '</button>' +
                '</div>' +
            '</div>';
        },

        step5Dns: function() {
            return '<div class="step step-5">' +
                '<h2>' + Lang.get('setup.dns.title') + '</h2>' +
                '<p class="subtitle">' + Lang.get('setup.dns.subtitle') + '</p>' +
                '<div class="dns-records" id="dns-records">' +
                    '<div class="loading">' + Lang.get('common.loading') + '</div>' +
                '</div>' +
                '<div class="buttons">' +
                    '<button class="btn secondary" id="back">' + Lang.get('setup.dns.back') + '</button>' +
                    '<button class="btn primary" id="next">' + Lang.get('setup.dns.next') + '</button>' +
                '</div>' +
            '</div>';
        },

        step6Done: function() {
            return '<div class="step step-6 done">' +
                '<div class="success-icon">&#10003;</div>' +
                '<h2>' + Lang.get('setup.done.title') + '</h2>' +
                '<p class="subtitle">' + Lang.get('setup.done.subtitle') + '</p>' +
                '<div class="buttons">' +
                    '<a href="/" class="btn primary">' + Lang.get('setup.done.admin') + '</a>' +
                '</div>' +
            '</div>';
        },

        bindEvents: function() {
            var self = this;
            var next = document.getElementById('next');
            var back = document.getElementById('back');

            if (next) next.addEventListener('click', function() { self.handleNext(); });
            if (back) back.addEventListener('click', function() { self.handleBack(); });

            var cards = document.querySelectorAll('.radio-card');
            cards.forEach(function(card) {
                card.addEventListener('click', function() {
                    cards.forEach(function(c) { c.classList.remove('selected'); });
                    card.classList.add('selected');
                    card.querySelector('input').checked = true;
                });
            });

            if (this.currentStep === 4) this.runAutoConfig();
            if (this.currentStep === 5) this.loadDnsRecords();
        },

        handleNext: async function() {
            var self = this;
            var endpoint, body;

            try {
                switch (this.currentStep) {
                    case 1:
                        var domain = document.getElementById('domain').value;
                        if (!domain) { alert('Please enter a domain'); return; }
                        endpoint = '/api/setup/domain';
                        body = JSON.stringify({ domain: domain });
                        break;

                    case 2:
                        var usage = document.querySelector('input[name="usage"]:checked').value;
                        endpoint = '/api/setup/usage';
                        body = JSON.stringify({ usageType: parseInt(usage) });
                        break;

                    case 3:
                        var email = document.getElementById('email').value;
                        var password = document.getElementById('password').value;
                        var confirm = document.getElementById('confirm').value;
                        if (password !== confirm) { alert('Passwords do not match'); return; }
                        if (password.length < 8) { alert('Password must be at least 8 characters'); return; }
                        endpoint = '/api/setup/admin';
                        body = JSON.stringify({ email: email, password: password });
                        break;

                    case 5:
                        endpoint = '/api/setup/complete';
                        body = JSON.stringify({});
                        break;

                    default:
                        this.currentStep++;
                        this.render();
                        return;
                }

                var res = await fetch(endpoint, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: body
                });

                var data = await res.json();
                if (data.success) {
                    this.currentStep++;
                    this.render();
                } else {
                    alert(data.error || 'Error');
                }
            } catch (e) {
                console.error(e);
                alert('Request failed');
            }
        },

        handleBack: function() {
            if (this.currentStep > 1) {
                this.currentStep--;
                this.render();
            }
        },

        runAutoConfig: async function() {
            try {
                var res = await fetch('/api/setup/autoconfig');
                var data = await res.json();

                if (data.success) {
                    var checks = document.getElementById('checks');
                    var config = data.data;
                    checks.innerHTML =
                        '<div class="check-item ' + (config.localIp ? 'ok' : 'warn') + '">' +
                            '<span class="icon">' + (config.localIp ? '&#10003;' : '&#9888;') + '</span>' +
                            Lang.get('setup.autoconfig.network') + ': ' + (config.localIp || 'Unknown') +
                        '</div>' +
                        '<div class="check-item ' + (config.port25 ? 'ok' : 'error') + '">' +
                            '<span class="icon">' + (config.port25 ? '&#10003;' : '&#10007;') + '</span>' +
                            Lang.get('setup.autoconfig.port25') +
                        '</div>' +
                        '<div class="check-item ' + (config.port587 ? 'ok' : 'error') + '">' +
                            '<span class="icon">' + (config.port587 ? '&#10003;' : '&#10007;') + '</span>' +
                            Lang.get('setup.autoconfig.port587') +
                        '</div>' +
                        '<div class="check-item ' + (config.port993 ? 'ok' : 'error') + '">' +
                            '<span class="icon">' + (config.port993 ? '&#10003;' : '&#10007;') + '</span>' +
                            Lang.get('setup.autoconfig.port993') +
                        '</div>' +
                        '<div class="check-item ' + (config.port995 ? 'ok' : 'error') + '">' +
                            '<span class="icon">' + (config.port995 ? '&#10003;' : '&#10007;') + '</span>' +
                            Lang.get('setup.autoconfig.port995') +
                        '</div>' +
                        '<div class="check-item ' + (config.certGenerated ? 'ok' : 'warn') + '">' +
                            '<span class="icon">' + (config.certGenerated ? '&#10003;' : '&#9888;') + '</span>' +
                            Lang.get('setup.autoconfig.cert') +
                        '</div>';
                    document.getElementById('next').disabled = false;
                }
            } catch (e) {
                console.error(e);
            }
        },

        loadDnsRecords: async function() {
            try {
                var res = await fetch('/api/setup/dns');
                var data = await res.json();

                if (data.success) {
                    var container = document.getElementById('dns-records');
                    var records = data.data.records;
                    var html = '';
                    for (var i = 0; i < records.length; i++) {
                        var r = records[i];
                        html += '<div class="dns-record">' +
                            '<span class="type">' + r.type + '</span>' +
                            '<span class="name">' + r.name + '</span>' +
                            '<span class="value">' + r.value + '</span>' +
                        '</div>';
                    }
                    container.innerHTML = html;
                }
            } catch (e) {
                console.error(e);
            }
        }
    };

    window.Setup = Setup;

    document.addEventListener('DOMContentLoaded', async function() {
        await App.init();
        Setup.init();
    });
})();
