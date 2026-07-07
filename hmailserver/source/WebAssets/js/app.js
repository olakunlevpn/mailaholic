(function() {
    'use strict';

    const Lang = {
        strings: {},
        async load(locale) {
            locale = locale || 'en';
            try {
                const res = await fetch('/js/lang/' + locale + '.json');
                this.strings = await res.json();
            } catch (e) {
                console.error('Failed to load language file:', e);
            }
        },
        get: function(key, replacements) {
            replacements = replacements || {};
            var str = this.strings[key] || key;
            for (var k in replacements) {
                if (replacements.hasOwnProperty(k)) {
                    str = str.replace('{' + k + '}', replacements[k]);
                }
            }
            return str;
        }
    };

    window.Lang = Lang;
    window.App = {
        init: async function() {
            await Lang.load('en');
        }
    };

    document.addEventListener('DOMContentLoaded', function() {
        App.init();
    });
})();
