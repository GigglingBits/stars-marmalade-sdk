/// <reference path="../../../typings/jquery/jquery.d.ts"/>
/// <reference path="jquery/jquery-1.10.2.js"/>

(function (window, $) {
    "use strict";

	//////////////////////////////////////////////////////////////////////
	// Initialize document
	//////////////////////////////////////////////////////////////////////
    $(function () {
		// event handlers
		$("body").delegate(".meemabbutton", "click", function () {
			onButtonClicked(this.id, this.textContent);
		});
		 
        // initialize meemab app
		clearData();
		setData("meemab-base-url", "http://meemab.cloudapp.net/api/");

		// start page
		showPageContent('pages/logon.html');
    });


	//////////////////////////////////////////////////////////////////////
	// DOM handling and navigation
	//////////////////////////////////////////////////////////////////////
	function onButtonClicked(buttonId, textContent) {
		logInfo("Button clicked: " + buttonId);
		switch (buttonId) {
			case 'signin':
				signIn();
				break;
			case 'showmetric':
				showMeasurement(textContent);
				break;
			default:			
		}
	}

	function showPageContent(filename) {
		$('#page').load(filename);
		logInfo("Loaded new page content: " + filename);
	}


	//////////////////////////////////////////////////////////////////////
	// State handling
	//////////////////////////////////////////////////////////////////////
	function clearData() {
		return window.sessionStorage.clear();
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
	// AJAX helpers
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
		setData("meemab-account", accountName);
		$('#sessioninfo').html('User: ' + getData("meemab-account"));
	}

	function getAccountName() {
		var accountName = getData("meemab-account");
		if (accountName == null || accountName.toString() == "") {
			accountName = queryAccountName();
		}
		setData("meemab-account", accountName);
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
		getServerData("Account", { AccountName: accountName }).fail(
			onCheckAccountFailure
		).done(
			onCheckAccountSucceeded
		);
		
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
		createServerData("Account", { AccountName: accountName}, data).fail(
			function () {
				logInfo("Account creation failed: " + JSON.stringify(status));		
			}
		).done(
			function () {
				logInfo("Account creation succeeded.");	
				authenticationCompleted();
			}
		);
	}

	function authenticationCompleted() {
		setAccountName(getData("meemab-account"));	
		logInfo("Authentication done.");
		buildMeemab();
		showPageContent('pages/capture.html');
	}

	//////////////////////////////////////////////////////////////////////
	// Meemab up- and download
	//////////////////////////////////////////////////////////////////////
	function buildMeemab() {
        $("#menupanel").empty().append(
        	$('<p>Loading your meemab...</p>')	
		);

		// get the meemab from the server		
		getServerData("Measures", { AccountName: getData("meemab-account")}).fail(
			function () {
		        $("#contentpanel").empty().append(
					$('<p>Failed: Meemab not found.</p>')	
				);
			}
		).then(
			function (meemab) {
				$("#contentpanel").empty().append(
					$('<p>Meemab loaded.</p>')
				);
				setData("meemab-measures", meemab);
			}
		);

		// get the list of measurables for the menu
		getServerData("Measurables", {}).fail(
			function () {
		        $("#menupanel").empty().append(
					$('<p>Failed: measurables was not loaded.</p>')	
				);
			}
		).then(
			function (measurables) {
				setData("meemab-measurables", measurables);
				showMenuPanel(measurables);
			}
		);
	}

	function showMenuPanel(measurables) {
        $("#menupanel").empty().append(
        	$('<p>Select a metric:</p>')	
		);
		measurables.forEach(function(measurable) {
            $("#menupanel").append(
                 $('<p id="showmetric" class="meemabbutton">' + measurable + '</p>')
            );
        });
	}

	//////////////////////////////////////////////////////////////////////
	// Measure capture
	//////////////////////////////////////////////////////////////////////
	function showMeasurement(metric) {
		logInfo("Show metric: " + metric);

		// get cached measures
		var meemab = getData("meemab-measures");
		var measure = meemab ? meemab[metric] : "please enter...";
		
		// enable data capture
		$("#contentpanel").empty().append(
			$('<p>Enter your measurement here. The new measurement is sent to the server when you leave the field.</p>')	
		).append(
			$('<div>' + metric + ':<input id="measurement" type="text" name="' + metric + '" value="' + measure + '"></input></div>')	
		);

		// save handler		
		$("#measurement").focusout(
			function(focusout) {
				updateMeasure(focusout.target.name, focusout.target.value);
			}
		);
	}	

	function updateMeasure(metric, measure) {
		// update cached measures
		var meemab = getData("meemab-measures");
		var oldmeasure = meemab ? meemab[metric] : "";
		
		if (oldmeasure != measure) {
			logInfo('Updating measure for "' + metric + '": "' + oldmeasure + '" -> "' + measure + '"');
			// todo: meemab.metric = measure;
			// todo: send meemab.metric to server
		} else {
			logInfo('Keeping measure for "' + metric + '": "' + oldmeasure + '"');			
		}		
	}

})(window, jQuery);
