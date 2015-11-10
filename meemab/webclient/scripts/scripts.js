/// <reference path="../../../typings/jquery/jquery.d.ts"/>
/// <reference path="jquery/jquery-1.10.2.js"/>

(function (window, $) {
    "use strict";

	//////////////////////////////////////////////////////////////////////
	// Initialization
	//////////////////////////////////////////////////////////////////////
    $(function () {
        // initialize meemab app
		attachEventHandlers();
		clearData();
		setData("meemab-base-url", "http://meemab.cloudapp.net/api/");
    });


	//////////////////////////////////////////////////////////////////////
	// DOM handling and navigation
	//////////////////////////////////////////////////////////////////////
	function attachEventHandlers() {
		$(function () {
			$("body").delegate(".meemabbutton", "click", function () {
				onButtonClicked(this.id);
			});
		});
	}

	function onButtonClicked(buttonid) {
		logInfo("Button clicked: " + buttonid);
		switch (buttonid) {
			case 'enter':
				showPageContent('fragments/logon.html');
				break;
			case 'signup':
				signIn();
				break;
			default:
				showMetricContent(buttonid);
		}
	}

	function showPageContent(filename) {
		$('#page').load(filename);
		logInfo("Loaded new page content: " + filename);
	}

	function showMetricContent(buttonid) {
		var filename = 'fragments/metrics/' + buttonid + '.html';
		$('#contentpanel').load(filename);
		logInfo("Loaded new metric content: " + filename);
	}


	//////////////////////////////////////////////////////////////////////
	// State handling
	//////////////////////////////////////////////////////////////////////
	function clearData() {
		return window.sessionStorage.clear;
	}

	function getData(key) {
		return window.sessionStorage[key];
	}

	function setData(key, data) {
		window.sessionStorage.setItem(key, data);
	}


	//////////////////////////////////////////////////////////////////////
	// debug/trace/log
	//////////////////////////////////////////////////////////////////////
	function logInfo(msg) {
		var strmsg = JSON.stringify(msg);
		console.info(strmsg);
	}

	function logLastRequest(msg) {
		var strmsg = JSON.stringify(msg);
		console.info(strmsg);
	}

	function logLastResponse(msg) {
		var strmsg = JSON.stringify(msg);
		console.info(strmsg);
	}


	//////////////////////////////////////////////////////////////////////
	// AJAX helper
	//////////////////////////////////////////////////////////////////////
	function getServerData(resource, params, body) {
		var url = getData("meemab-base-url") + resource + "?" + $.param(params);

		logLastRequest({ url: url, data: body });

		return $.getJSON(url, body)
			.always(logLastResponse);
	}

	function createServerData(resource, params, body) {
		var url = getData("meemab-base-url") + resource + "?" + $.param(params);

		logLastRequest({ url: url, data: body });

		return $.post(url, body)
			.always(logLastResponse);
	}

	function updateServerData(resource, body) {
		var url = getData("meemab-base-url") + resource + "?" + $.param(params);

		logLastRequest({ url: url, data: body });

		return $.put(url, body)
			.always(logLastResponse);
	}


	//////////////////////////////////////////////////////////////////////
	// Sign-in handling
	//////////////////////////////////////////////////////////////////////
	function setAccountName(accountName) {
		setData("account", accountName);
		$('#sessioninfo').html('User: ' + getData("account"));
	}

	function getAccountName() {
		var accountName = getData("account");
		if (accountName == null || accountName.toString() == "") {
			accountName = queryAccountName();
		}
		setData("account", accountName);
		return accountName;
	}

	function signIn() {
		var accountName = getAccountName();
		checkAccount(accountName);
		return true;
	}

	function queryAccountName() {
		var account = "";
		while (account == null || account == "") {
			account = prompt('Please enter your account name', 'peter.pan@meemab.com');
		}
		return account;
	}

	function checkAccount(accountName) {
		// check if account exists
		logInfo("Creating account: " + accountName);
		getServerData("Account", { AccountName: accountName }).fail(onCheckAccountFailure).done(onCheckAccountSucceeded);
		
		// ignore the result, if the account is found
		// only the error case is interesting for now
	}

	function onCheckAccountFailure(response) {
		if (response.status == 404) {
			logInfo("Account is unknown or lookup failed. Trying to create account...");
			createAccount(getAccountName());
		}
	}

	function onCheckAccountSucceeded() {
		logInfo("Account already exists.");	
		authenticationCompleted();
	}

	function createAccount(accountName) {
		var data = {
			"AccountName": accountName,
			"Email": accountName,
			"Gender": "M",
			"BirthDate": "1976-01-23T07:11:26.1045798+00:00",
			"Country": "Denmark"
		}
		createServerData("Account", { AccountName: accountName}, data).fail(onCreateAccountFailed).done(onCreateAccountSucceeded);
	}

	function onCreateAccountSucceeded() {
		logInfo("Account creation succeeded.");	
		authenticationCompleted();
	}

	function onCreateAccountFailed(response) {
		logInfo("Account creation failed: " + JSON.stringify(status));
	}

	function authenticationCompleted() {
		setAccountName(getData("account"));	
		showPageContent('fragments/main.html');
	}

	//////////////////////////////////////////////////////////////////////
	// Meemab up- and download
	//////////////////////////////////////////////////////////////////////


})(window, jQuery);
