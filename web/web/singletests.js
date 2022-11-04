function singletests() { 
 document.requesthelper.singletest.options.length=0 
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stWalletTest") { 
 var opt = document.createElement("option")
 opt.value = "dayLimitConstruction"
 opt.text = "dayLimitConstruction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "dayLimitConstructionOOG"
 opt.text = "dayLimitConstructionOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "dayLimitConstructionPartial"
 opt.text = "dayLimitConstructionPartial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "dayLimitResetSpentToday"
 opt.text = "dayLimitResetSpentToday"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "dayLimitSetDailyLimit"
 opt.text = "dayLimitSetDailyLimit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "dayLimitSetDailyLimitNoData"
 opt.text = "dayLimitSetDailyLimitNoData"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedAddOwnerAddMyself"
 opt.text = "multiOwnedAddOwnerAddMyself"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedAddOwner"
 opt.text = "multiOwnedAddOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedChangeOwner"
 opt.text = "multiOwnedChangeOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedChangeOwnerNoArgument"
 opt.text = "multiOwnedChangeOwnerNoArgument"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedChangeOwner_fromNotOwner"
 opt.text = "multiOwnedChangeOwner_fromNotOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedChangeOwner_toIsOwner"
 opt.text = "multiOwnedChangeOwner_toIsOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedChangeRequirementTo0"
 opt.text = "multiOwnedChangeRequirementTo0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedChangeRequirementTo1"
 opt.text = "multiOwnedChangeRequirementTo1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedChangeRequirementTo2"
 opt.text = "multiOwnedChangeRequirementTo2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedConstructionCorrect"
 opt.text = "multiOwnedConstructionCorrect"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedConstructionNotEnoughGas"
 opt.text = "multiOwnedConstructionNotEnoughGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedConstructionNotEnoughGasPartial"
 opt.text = "multiOwnedConstructionNotEnoughGasPartial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedIsOwnerFalse"
 opt.text = "multiOwnedIsOwnerFalse"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedIsOwnerTrue"
 opt.text = "multiOwnedIsOwnerTrue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedRemoveOwnerByNonOwner"
 opt.text = "multiOwnedRemoveOwnerByNonOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedRemoveOwner"
 opt.text = "multiOwnedRemoveOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedRemoveOwner_mySelf"
 opt.text = "multiOwnedRemoveOwner_mySelf"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedRemoveOwner_ownerIsNotOwner"
 opt.text = "multiOwnedRemoveOwner_ownerIsNotOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "multiOwnedRevokeNothing"
 opt.text = "multiOwnedRevokeNothing"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletAddOwnerRemovePendingTransaction"
 opt.text = "walletAddOwnerRemovePendingTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletChangeOwnerRemovePendingTransaction"
 opt.text = "walletChangeOwnerRemovePendingTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletChangeRequirementRemovePendingTransaction"
 opt.text = "walletChangeRequirementRemovePendingTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletConfirm"
 opt.text = "walletConfirm"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletConstruction"
 opt.text = "walletConstruction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletConstructionOOG"
 opt.text = "walletConstructionOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletConstructionPartial"
 opt.text = "walletConstructionPartial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletDefault"
 opt.text = "walletDefault"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletDefaultWithOutValue"
 opt.text = "walletDefaultWithOutValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletExecuteOverDailyLimitMultiOwner"
 opt.text = "walletExecuteOverDailyLimitMultiOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletExecuteOverDailyLimitOnlyOneOwner"
 opt.text = "walletExecuteOverDailyLimitOnlyOneOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletExecuteOverDailyLimitOnlyOneOwnerNew"
 opt.text = "walletExecuteOverDailyLimitOnlyOneOwnerNew"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletExecuteUnderDailyLimit"
 opt.text = "walletExecuteUnderDailyLimit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletKill"
 opt.text = "walletKill"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletKillNotByOwner"
 opt.text = "walletKillNotByOwner"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletKillToWallet"
 opt.text = "walletKillToWallet"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "walletRemoveOwnerRemovePendingTransaction"
 opt.text = "walletRemoveOwnerRemovePendingTransaction"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stBugs") { 
 var opt = document.createElement("option")
 opt.value = "evmBytecode"
 opt.text = "evmBytecode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetestDEFAULT-Tue_07_58_41-15153-575192"
 opt.text = "randomStatetestDEFAULT-Tue_07_58_41-15153-575192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetestDEFAULT-Tue_07_58_41-15153-575192_london"
 opt.text = "randomStatetestDEFAULT-Tue_07_58_41-15153-575192_london"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopyPythonBug_Tue_03_48_41-1432"
 opt.text = "returndatacopyPythonBug_Tue_03_48_41-1432"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "staticcall_createfails"
 opt.text = "staticcall_createfails"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stEIP1559") { 
 var opt = document.createElement("option")
 opt.value = "baseFeeDiffPlaces"
 opt.text = "baseFeeDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasPriceDiffPlaces"
 opt.text = "gasPriceDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "intrinsic"
 opt.text = "intrinsic"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "lowFeeCap"
 opt.text = "lowFeeCap"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "lowGasLimit"
 opt.text = "lowGasLimit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "lowGasPriceOldTypes"
 opt.text = "lowGasPriceOldTypes"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "outOfFunds"
 opt.text = "outOfFunds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "outOfFundsOldTypes"
 opt.text = "outOfFundsOldTypes"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "senderBalance"
 opt.text = "senderBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "tipTooHigh"
 opt.text = "tipTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "transactionIntinsicBug"
 opt.text = "transactionIntinsicBug"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "typeTwoBerlin"
 opt.text = "typeTwoBerlin"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "valCausesOOF"
 opt.text = "valCausesOOF"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stDelegatecallTestHomestead") { 
 var opt = document.createElement("option")
 opt.value = "Call1024BalanceTooLow"
 opt.text = "Call1024BalanceTooLow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call1024OOG"
 opt.text = "Call1024OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call1024PreCalls"
 opt.text = "Call1024PreCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallLoseGasOOG"
 opt.text = "CallLoseGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBombPreCall"
 opt.text = "CallRecursiveBombPreCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallcodeLoseGasOOG"
 opt.text = "CallcodeLoseGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Delegatecall1024"
 opt.text = "Delegatecall1024"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Delegatecall1024OOG"
 opt.text = "Delegatecall1024OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput1"
 opt.text = "callOutput1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput2"
 opt.text = "callOutput2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput3"
 opt.text = "callOutput3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput3partialFail"
 opt.text = "callOutput3partialFail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput3partial"
 opt.text = "callOutput3partial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callWithHighValueAndGasOOG"
 opt.text = "callWithHighValueAndGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeOutput3"
 opt.text = "callcodeOutput3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeWithHighValueAndGasOOG"
 opt.text = "callcodeWithHighValueAndGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "deleagateCallAfterValueTransfer"
 opt.text = "deleagateCallAfterValueTransfer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallAndOOGatTxLevel"
 opt.text = "delegatecallAndOOGatTxLevel"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallBasic"
 opt.text = "delegatecallBasic"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallEmptycontract"
 opt.text = "delegatecallEmptycontract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallInInitcodeToEmptyContract"
 opt.text = "delegatecallInInitcodeToEmptyContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallInInitcodeToExistingContract"
 opt.text = "delegatecallInInitcodeToExistingContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallInInitcodeToExistingContractOOG"
 opt.text = "delegatecallInInitcodeToExistingContractOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallOOGinCall"
 opt.text = "delegatecallOOGinCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallSenderCheck"
 opt.text = "delegatecallSenderCheck"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallValueCheck"
 opt.text = "delegatecallValueCheck"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecodeDynamicCode2SelfCall"
 opt.text = "delegatecodeDynamicCode2SelfCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecodeDynamicCode"
 opt.text = "delegatecodeDynamicCode"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stChainId") { 
 var opt = document.createElement("option")
 opt.value = "chainId"
 opt.text = "chainId"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "chainIdGasCost"
 opt.text = "chainIdGasCost"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stExtCodeHash") { 
 var opt = document.createElement("option")
 opt.value = "callToNonExistent"
 opt.text = "callToNonExistent"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callToSuicideThenExtcodehash"
 opt.text = "callToSuicideThenExtcodehash"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "codeCopyZero"
 opt.text = "codeCopyZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createEmptyThenExtcodehash"
 opt.text = "createEmptyThenExtcodehash"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "dynamicAccountOverwriteEmpty"
 opt.text = "dynamicAccountOverwriteEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeCopyBounds"
 opt.text = "extCodeCopyBounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashAccountWithoutCode"
 opt.text = "extCodeHashAccountWithoutCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashCALLCODE"
 opt.text = "extCodeHashCALLCODE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashCALL"
 opt.text = "extCodeHashCALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashChangedAccount"
 opt.text = "extCodeHashChangedAccount"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashCreatedAndDeletedAccountCall"
 opt.text = "extCodeHashCreatedAndDeletedAccountCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashCreatedAndDeletedAccount"
 opt.text = "extCodeHashCreatedAndDeletedAccount"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashCreatedAndDeletedAccountRecheckInOuterCall"
 opt.text = "extCodeHashCreatedAndDeletedAccountRecheckInOuterCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashCreatedAndDeletedAccountStaticCall"
 opt.text = "extCodeHashCreatedAndDeletedAccountStaticCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashDELEGATECALL"
 opt.text = "extCodeHashDELEGATECALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashDeletedAccount1"
 opt.text = "extCodeHashDeletedAccount1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashDeletedAccount2"
 opt.text = "extCodeHashDeletedAccount2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashDeletedAccount3"
 opt.text = "extCodeHashDeletedAccount3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashDeletedAccount4"
 opt.text = "extCodeHashDeletedAccount4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashDeletedAccount"
 opt.text = "extCodeHashDeletedAccount"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashDynamicArgument"
 opt.text = "extCodeHashDynamicArgument"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashInInitCode"
 opt.text = "extCodeHashInInitCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashMaxCodeSize"
 opt.text = "extCodeHashMaxCodeSize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashNewAccount"
 opt.text = "extCodeHashNewAccount"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashNonExistingAccount"
 opt.text = "extCodeHashNonExistingAccount"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashPrecompiles"
 opt.text = "extCodeHashPrecompiles"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashSTATICCALL"
 opt.text = "extCodeHashSTATICCALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashSelf"
 opt.text = "extCodeHashSelf"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashSelfInInit"
 opt.text = "extCodeHashSelfInInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashSubcallOOG"
 opt.text = "extCodeHashSubcallOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extCodeHashSubcallSuicide"
 opt.text = "extCodeHashSubcallSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extcodehashEmpty"
 opt.text = "extcodehashEmpty"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stZeroCallsRevert") { 
 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALLCODE_OOGRevert"
 opt.text = "ZeroValue_CALLCODE_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALLCODE_ToEmpty_OOGRevert"
 opt.text = "ZeroValue_CALLCODE_ToEmpty_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALLCODE_ToNonZeroBalance_OOGRevert"
 opt.text = "ZeroValue_CALLCODE_ToNonZeroBalance_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALLCODE_ToOneStorageKey_OOGRevert"
 opt.text = "ZeroValue_CALLCODE_ToOneStorageKey_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALL_OOGRevert"
 opt.text = "ZeroValue_CALL_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALL_ToEmpty_OOGRevert"
 opt.text = "ZeroValue_CALL_ToEmpty_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALL_ToNonZeroBalance_OOGRevert"
 opt.text = "ZeroValue_CALL_ToNonZeroBalance_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALL_ToOneStorageKey_OOGRevert"
 opt.text = "ZeroValue_CALL_ToOneStorageKey_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_DELEGATECALL_OOGRevert"
 opt.text = "ZeroValue_DELEGATECALL_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_DELEGATECALL_ToEmpty_OOGRevert"
 opt.text = "ZeroValue_DELEGATECALL_ToEmpty_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_DELEGATECALL_ToNonZeroBalance_OOGRevert"
 opt.text = "ZeroValue_DELEGATECALL_ToNonZeroBalance_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_DELEGATECALL_ToOneStorageKey_OOGRevert"
 opt.text = "ZeroValue_DELEGATECALL_ToOneStorageKey_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_SUICIDE_OOGRevert"
 opt.text = "ZeroValue_SUICIDE_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_SUICIDE_ToEmpty_OOGRevert"
 opt.text = "ZeroValue_SUICIDE_ToEmpty_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_SUICIDE_ToNonZeroBalance_OOGRevert"
 opt.text = "ZeroValue_SUICIDE_ToNonZeroBalance_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_SUICIDE_ToOneStorageKey_OOGRevert"
 opt.text = "ZeroValue_SUICIDE_ToOneStorageKey_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stSelfBalance") { 
 var opt = document.createElement("option")
 opt.value = "diffPlaces"
 opt.text = "diffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "selfBalanceCallTypes"
 opt.text = "selfBalanceCallTypes"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "selfBalanceEqualsBalance"
 opt.text = "selfBalanceEqualsBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "selfBalance"
 opt.text = "selfBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "selfBalanceGasCost"
 opt.text = "selfBalanceGasCost"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "selfBalanceUpdate"
 opt.text = "selfBalanceUpdate"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stQuadraticComplexityTest") { 
 var opt = document.createElement("option")
 opt.value = "Call1MB1024Calldepth"
 opt.text = "Call1MB1024Calldepth"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call20KbytesContract50_1"
 opt.text = "Call20KbytesContract50_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call20KbytesContract50_2"
 opt.text = "Call20KbytesContract50_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call20KbytesContract50_3"
 opt.text = "Call20KbytesContract50_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call50000"
 opt.text = "Call50000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call50000_ecrec"
 opt.text = "Call50000_ecrec"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call50000_identity2"
 opt.text = "Call50000_identity2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call50000_identity"
 opt.text = "Call50000_identity"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call50000_rip160"
 opt.text = "Call50000_rip160"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call50000_sha256"
 opt.text = "Call50000_sha256"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Callcode50000"
 opt.text = "Callcode50000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create1000Byzantium"
 opt.text = "Create1000Byzantium"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create1000"
 opt.text = "Create1000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "QuadraticComplexitySolidity_CallDataCopy"
 opt.text = "QuadraticComplexitySolidity_CallDataCopy"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Return50000"
 opt.text = "Return50000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Return50000_2"
 opt.text = "Return50000_2"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stCodeSizeLimit") { 
 var opt = document.createElement("option")
 opt.value = "codesizeInit"
 opt.text = "codesizeInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "codesizeOOGInvalidSize"
 opt.text = "codesizeOOGInvalidSize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "codesizeValid"
 opt.text = "codesizeValid"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2CodeSizeLimit"
 opt.text = "create2CodeSizeLimit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createCodeSizeLimit"
 opt.text = "createCodeSizeLimit"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stReturnDataTest") { 
 var opt = document.createElement("option")
 opt.value = "call_ecrec_success_empty_then_returndatasize"
 opt.text = "call_ecrec_success_empty_then_returndatasize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "call_outsize_then_create_successful_then_returndatasize"
 opt.text = "call_outsize_then_create_successful_then_returndatasize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "call_then_call_value_fail_then_returndatasize"
 opt.text = "call_then_call_value_fail_then_returndatasize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "call_then_create_successful_then_returndatasize"
 opt.text = "call_then_create_successful_then_returndatasize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "clearReturnBuffer"
 opt.text = "clearReturnBuffer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create_callprecompile_returndatasize"
 opt.text = "create_callprecompile_returndatasize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modexp_modsize0_returndatasize"
 opt.text = "modexp_modsize0_returndatasize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_0_0_following_successful_create"
 opt.text = "returndatacopy_0_0_following_successful_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_afterFailing_create"
 opt.text = "returndatacopy_afterFailing_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_after_failing_callcode"
 opt.text = "returndatacopy_after_failing_callcode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_after_failing_delegatecall"
 opt.text = "returndatacopy_after_failing_delegatecall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_after_failing_staticcall"
 opt.text = "returndatacopy_after_failing_staticcall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_after_revert_in_staticcall"
 opt.text = "returndatacopy_after_revert_in_staticcall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_after_successful_callcode"
 opt.text = "returndatacopy_after_successful_callcode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_after_successful_delegatecall"
 opt.text = "returndatacopy_after_successful_delegatecall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_after_successful_staticcall"
 opt.text = "returndatacopy_after_successful_staticcall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_call"
 opt.text = "returndatacopy_following_call"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_create"
 opt.text = "returndatacopy_following_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_failing_call"
 opt.text = "returndatacopy_following_failing_call"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_revert"
 opt.text = "returndatacopy_following_revert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_revert_in_create"
 opt.text = "returndatacopy_following_revert_in_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_successful_create"
 opt.text = "returndatacopy_following_successful_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_too_big_transfer"
 opt.text = "returndatacopy_following_too_big_transfer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_initial"
 opt.text = "returndatacopy_initial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_initial_256"
 opt.text = "returndatacopy_initial_256"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_initial_big_sum"
 opt.text = "returndatacopy_initial_big_sum"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_overrun"
 opt.text = "returndatacopy_overrun"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_after_failing_callcode"
 opt.text = "returndatasize_after_failing_callcode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_after_failing_delegatecall"
 opt.text = "returndatasize_after_failing_delegatecall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_after_failing_staticcall"
 opt.text = "returndatasize_after_failing_staticcall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_after_oog_after_deeper"
 opt.text = "returndatasize_after_oog_after_deeper"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_after_successful_callcode"
 opt.text = "returndatasize_after_successful_callcode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_after_successful_delegatecall"
 opt.text = "returndatasize_after_successful_delegatecall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_after_successful_staticcall"
 opt.text = "returndatasize_after_successful_staticcall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_bug"
 opt.text = "returndatasize_bug"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_following_successful_create"
 opt.text = "returndatasize_following_successful_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_initial"
 opt.text = "returndatasize_initial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_initial_zero_read"
 opt.text = "returndatasize_initial_zero_read"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "revertRetDataSize"
 opt.text = "revertRetDataSize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "subcallReturnMoreThenExpected"
 opt.text = "subcallReturnMoreThenExpected"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "tooLongReturnDataCopy"
 opt.text = "tooLongReturnDataCopy"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stRandom") { 
 var opt = document.createElement("option")
 opt.value = "randomStatetest0"
 opt.text = "randomStatetest0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest100"
 opt.text = "randomStatetest100"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest101"
 opt.text = "randomStatetest101"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest102"
 opt.text = "randomStatetest102"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest103"
 opt.text = "randomStatetest103"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest104"
 opt.text = "randomStatetest104"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest105"
 opt.text = "randomStatetest105"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest106"
 opt.text = "randomStatetest106"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest107"
 opt.text = "randomStatetest107"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest108"
 opt.text = "randomStatetest108"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest10"
 opt.text = "randomStatetest10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest110"
 opt.text = "randomStatetest110"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest111"
 opt.text = "randomStatetest111"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest112"
 opt.text = "randomStatetest112"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest114"
 opt.text = "randomStatetest114"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest115"
 opt.text = "randomStatetest115"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest116"
 opt.text = "randomStatetest116"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest117"
 opt.text = "randomStatetest117"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest118"
 opt.text = "randomStatetest118"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest119"
 opt.text = "randomStatetest119"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest11"
 opt.text = "randomStatetest11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest120"
 opt.text = "randomStatetest120"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest121"
 opt.text = "randomStatetest121"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest122"
 opt.text = "randomStatetest122"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest124"
 opt.text = "randomStatetest124"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest125"
 opt.text = "randomStatetest125"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest126"
 opt.text = "randomStatetest126"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest129"
 opt.text = "randomStatetest129"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest12"
 opt.text = "randomStatetest12"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest130"
 opt.text = "randomStatetest130"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest131"
 opt.text = "randomStatetest131"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest133"
 opt.text = "randomStatetest133"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest134"
 opt.text = "randomStatetest134"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest135"
 opt.text = "randomStatetest135"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest137"
 opt.text = "randomStatetest137"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest138"
 opt.text = "randomStatetest138"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest139"
 opt.text = "randomStatetest139"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest13"
 opt.text = "randomStatetest13"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest142"
 opt.text = "randomStatetest142"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest143"
 opt.text = "randomStatetest143"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest144"
 opt.text = "randomStatetest144"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest145"
 opt.text = "randomStatetest145"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest146"
 opt.text = "randomStatetest146"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest147"
 opt.text = "randomStatetest147"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest148"
 opt.text = "randomStatetest148"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest149"
 opt.text = "randomStatetest149"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest14"
 opt.text = "randomStatetest14"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest150"
 opt.text = "randomStatetest150"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest151"
 opt.text = "randomStatetest151"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest153"
 opt.text = "randomStatetest153"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest154"
 opt.text = "randomStatetest154"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest155"
 opt.text = "randomStatetest155"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest156"
 opt.text = "randomStatetest156"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest157"
 opt.text = "randomStatetest157"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest158"
 opt.text = "randomStatetest158"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest159"
 opt.text = "randomStatetest159"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest15"
 opt.text = "randomStatetest15"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest161"
 opt.text = "randomStatetest161"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest162"
 opt.text = "randomStatetest162"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest163"
 opt.text = "randomStatetest163"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest164"
 opt.text = "randomStatetest164"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest166"
 opt.text = "randomStatetest166"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest167"
 opt.text = "randomStatetest167"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest169"
 opt.text = "randomStatetest169"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest16"
 opt.text = "randomStatetest16"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest171"
 opt.text = "randomStatetest171"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest172"
 opt.text = "randomStatetest172"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest173"
 opt.text = "randomStatetest173"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest174"
 opt.text = "randomStatetest174"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest175"
 opt.text = "randomStatetest175"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest176"
 opt.text = "randomStatetest176"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest177"
 opt.text = "randomStatetest177"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest178"
 opt.text = "randomStatetest178"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest179"
 opt.text = "randomStatetest179"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest17"
 opt.text = "randomStatetest17"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest180"
 opt.text = "randomStatetest180"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest183"
 opt.text = "randomStatetest183"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest184"
 opt.text = "randomStatetest184"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest185"
 opt.text = "randomStatetest185"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest187"
 opt.text = "randomStatetest187"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest188"
 opt.text = "randomStatetest188"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest189"
 opt.text = "randomStatetest189"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest18"
 opt.text = "randomStatetest18"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest190"
 opt.text = "randomStatetest190"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest191"
 opt.text = "randomStatetest191"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest192"
 opt.text = "randomStatetest192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest194"
 opt.text = "randomStatetest194"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest195"
 opt.text = "randomStatetest195"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest196"
 opt.text = "randomStatetest196"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest197"
 opt.text = "randomStatetest197"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest198"
 opt.text = "randomStatetest198"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest199"
 opt.text = "randomStatetest199"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest19"
 opt.text = "randomStatetest19"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest1"
 opt.text = "randomStatetest1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest200"
 opt.text = "randomStatetest200"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest201"
 opt.text = "randomStatetest201"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest202"
 opt.text = "randomStatetest202"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest204"
 opt.text = "randomStatetest204"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest205"
 opt.text = "randomStatetest205"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest206"
 opt.text = "randomStatetest206"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest207"
 opt.text = "randomStatetest207"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest208"
 opt.text = "randomStatetest208"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest209"
 opt.text = "randomStatetest209"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest20"
 opt.text = "randomStatetest20"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest210"
 opt.text = "randomStatetest210"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest211"
 opt.text = "randomStatetest211"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest212"
 opt.text = "randomStatetest212"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest214"
 opt.text = "randomStatetest214"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest215"
 opt.text = "randomStatetest215"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest216"
 opt.text = "randomStatetest216"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest217"
 opt.text = "randomStatetest217"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest219"
 opt.text = "randomStatetest219"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest220"
 opt.text = "randomStatetest220"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest221"
 opt.text = "randomStatetest221"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest222"
 opt.text = "randomStatetest222"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest225"
 opt.text = "randomStatetest225"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest226"
 opt.text = "randomStatetest226"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest227"
 opt.text = "randomStatetest227"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest228"
 opt.text = "randomStatetest228"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest22"
 opt.text = "randomStatetest22"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest230"
 opt.text = "randomStatetest230"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest231"
 opt.text = "randomStatetest231"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest232"
 opt.text = "randomStatetest232"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest233"
 opt.text = "randomStatetest233"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest236"
 opt.text = "randomStatetest236"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest237"
 opt.text = "randomStatetest237"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest238"
 opt.text = "randomStatetest238"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest23"
 opt.text = "randomStatetest23"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest242"
 opt.text = "randomStatetest242"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest243"
 opt.text = "randomStatetest243"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest244"
 opt.text = "randomStatetest244"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest245"
 opt.text = "randomStatetest245"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest246"
 opt.text = "randomStatetest246"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest247"
 opt.text = "randomStatetest247"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest248"
 opt.text = "randomStatetest248"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest249"
 opt.text = "randomStatetest249"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest24"
 opt.text = "randomStatetest24"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest250"
 opt.text = "randomStatetest250"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest251"
 opt.text = "randomStatetest251"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest252"
 opt.text = "randomStatetest252"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest254"
 opt.text = "randomStatetest254"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest257"
 opt.text = "randomStatetest257"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest259"
 opt.text = "randomStatetest259"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest25"
 opt.text = "randomStatetest25"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest260"
 opt.text = "randomStatetest260"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest261"
 opt.text = "randomStatetest261"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest263"
 opt.text = "randomStatetest263"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest264"
 opt.text = "randomStatetest264"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest265"
 opt.text = "randomStatetest265"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest266"
 opt.text = "randomStatetest266"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest267"
 opt.text = "randomStatetest267"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest268"
 opt.text = "randomStatetest268"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest269"
 opt.text = "randomStatetest269"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest26"
 opt.text = "randomStatetest26"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest270"
 opt.text = "randomStatetest270"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest271"
 opt.text = "randomStatetest271"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest273"
 opt.text = "randomStatetest273"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest274"
 opt.text = "randomStatetest274"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest275"
 opt.text = "randomStatetest275"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest276"
 opt.text = "randomStatetest276"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest278"
 opt.text = "randomStatetest278"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest279"
 opt.text = "randomStatetest279"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest27"
 opt.text = "randomStatetest27"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest280"
 opt.text = "randomStatetest280"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest281"
 opt.text = "randomStatetest281"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest282"
 opt.text = "randomStatetest282"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest283"
 opt.text = "randomStatetest283"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest285"
 opt.text = "randomStatetest285"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest286"
 opt.text = "randomStatetest286"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest287"
 opt.text = "randomStatetest287"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest288"
 opt.text = "randomStatetest288"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest28"
 opt.text = "randomStatetest28"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest290"
 opt.text = "randomStatetest290"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest291"
 opt.text = "randomStatetest291"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest292"
 opt.text = "randomStatetest292"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest293"
 opt.text = "randomStatetest293"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest294"
 opt.text = "randomStatetest294"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest295"
 opt.text = "randomStatetest295"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest296"
 opt.text = "randomStatetest296"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest297"
 opt.text = "randomStatetest297"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest298"
 opt.text = "randomStatetest298"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest299"
 opt.text = "randomStatetest299"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest29"
 opt.text = "randomStatetest29"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest2"
 opt.text = "randomStatetest2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest300"
 opt.text = "randomStatetest300"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest301"
 opt.text = "randomStatetest301"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest302"
 opt.text = "randomStatetest302"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest303"
 opt.text = "randomStatetest303"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest304"
 opt.text = "randomStatetest304"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest305"
 opt.text = "randomStatetest305"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest306"
 opt.text = "randomStatetest306"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest307"
 opt.text = "randomStatetest307"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest308"
 opt.text = "randomStatetest308"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest309"
 opt.text = "randomStatetest309"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest30"
 opt.text = "randomStatetest30"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest310"
 opt.text = "randomStatetest310"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest311"
 opt.text = "randomStatetest311"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest312"
 opt.text = "randomStatetest312"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest313"
 opt.text = "randomStatetest313"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest315"
 opt.text = "randomStatetest315"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest316"
 opt.text = "randomStatetest316"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest318"
 opt.text = "randomStatetest318"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest31"
 opt.text = "randomStatetest31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest320"
 opt.text = "randomStatetest320"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest321"
 opt.text = "randomStatetest321"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest322"
 opt.text = "randomStatetest322"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest323"
 opt.text = "randomStatetest323"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest325"
 opt.text = "randomStatetest325"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest326"
 opt.text = "randomStatetest326"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest327"
 opt.text = "randomStatetest327"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest329"
 opt.text = "randomStatetest329"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest32"
 opt.text = "randomStatetest32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest332"
 opt.text = "randomStatetest332"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest333"
 opt.text = "randomStatetest333"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest334"
 opt.text = "randomStatetest334"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest335"
 opt.text = "randomStatetest335"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest336"
 opt.text = "randomStatetest336"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest337"
 opt.text = "randomStatetest337"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest338"
 opt.text = "randomStatetest338"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest339"
 opt.text = "randomStatetest339"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest33"
 opt.text = "randomStatetest33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest340"
 opt.text = "randomStatetest340"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest341"
 opt.text = "randomStatetest341"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest342"
 opt.text = "randomStatetest342"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest343"
 opt.text = "randomStatetest343"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest345"
 opt.text = "randomStatetest345"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest346"
 opt.text = "randomStatetest346"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest347"
 opt.text = "randomStatetest347"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest348"
 opt.text = "randomStatetest348"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest349"
 opt.text = "randomStatetest349"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest350"
 opt.text = "randomStatetest350"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest351"
 opt.text = "randomStatetest351"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest352"
 opt.text = "randomStatetest352"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest353"
 opt.text = "randomStatetest353"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest354"
 opt.text = "randomStatetest354"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest355"
 opt.text = "randomStatetest355"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest356"
 opt.text = "randomStatetest356"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest357"
 opt.text = "randomStatetest357"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest358"
 opt.text = "randomStatetest358"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest359"
 opt.text = "randomStatetest359"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest360"
 opt.text = "randomStatetest360"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest361"
 opt.text = "randomStatetest361"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest362"
 opt.text = "randomStatetest362"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest363"
 opt.text = "randomStatetest363"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest364"
 opt.text = "randomStatetest364"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest365"
 opt.text = "randomStatetest365"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest366"
 opt.text = "randomStatetest366"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest367"
 opt.text = "randomStatetest367"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest368"
 opt.text = "randomStatetest368"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest369"
 opt.text = "randomStatetest369"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest36"
 opt.text = "randomStatetest36"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest370"
 opt.text = "randomStatetest370"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest371"
 opt.text = "randomStatetest371"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest372"
 opt.text = "randomStatetest372"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest376"
 opt.text = "randomStatetest376"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest378"
 opt.text = "randomStatetest378"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest379"
 opt.text = "randomStatetest379"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest37"
 opt.text = "randomStatetest37"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest380"
 opt.text = "randomStatetest380"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest381"
 opt.text = "randomStatetest381"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest382"
 opt.text = "randomStatetest382"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest383"
 opt.text = "randomStatetest383"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest384"
 opt.text = "randomStatetest384"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest39"
 opt.text = "randomStatetest39"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest3"
 opt.text = "randomStatetest3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest41"
 opt.text = "randomStatetest41"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest42"
 opt.text = "randomStatetest42"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest43"
 opt.text = "randomStatetest43"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest45"
 opt.text = "randomStatetest45"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest47"
 opt.text = "randomStatetest47"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest48"
 opt.text = "randomStatetest48"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest49"
 opt.text = "randomStatetest49"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest4"
 opt.text = "randomStatetest4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest50"
 opt.text = "randomStatetest50"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest51"
 opt.text = "randomStatetest51"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest52"
 opt.text = "randomStatetest52"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest53"
 opt.text = "randomStatetest53"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest54"
 opt.text = "randomStatetest54"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest55"
 opt.text = "randomStatetest55"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest57"
 opt.text = "randomStatetest57"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest58"
 opt.text = "randomStatetest58"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest59"
 opt.text = "randomStatetest59"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest5"
 opt.text = "randomStatetest5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest60"
 opt.text = "randomStatetest60"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest62"
 opt.text = "randomStatetest62"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest63"
 opt.text = "randomStatetest63"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest64"
 opt.text = "randomStatetest64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest66"
 opt.text = "randomStatetest66"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest67"
 opt.text = "randomStatetest67"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest69"
 opt.text = "randomStatetest69"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest6"
 opt.text = "randomStatetest6"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest72"
 opt.text = "randomStatetest72"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest73"
 opt.text = "randomStatetest73"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest74"
 opt.text = "randomStatetest74"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest75"
 opt.text = "randomStatetest75"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest77"
 opt.text = "randomStatetest77"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest78"
 opt.text = "randomStatetest78"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest7"
 opt.text = "randomStatetest7"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest80"
 opt.text = "randomStatetest80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest81"
 opt.text = "randomStatetest81"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest82"
 opt.text = "randomStatetest82"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest83"
 opt.text = "randomStatetest83"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest84"
 opt.text = "randomStatetest84"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest85"
 opt.text = "randomStatetest85"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest87"
 opt.text = "randomStatetest87"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest88"
 opt.text = "randomStatetest88"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest89"
 opt.text = "randomStatetest89"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest90"
 opt.text = "randomStatetest90"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest92"
 opt.text = "randomStatetest92"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest95"
 opt.text = "randomStatetest95"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest96"
 opt.text = "randomStatetest96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest97"
 opt.text = "randomStatetest97"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest98"
 opt.text = "randomStatetest98"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest9"
 opt.text = "randomStatetest9"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stPreCompiledContracts2") { 
 var opt = document.createElement("option")
 opt.value = "CALLBlake2f"
 opt.text = "CALLBlake2f"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEBlake2f"
 opt.text = "CALLCODEBlake2f"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover0"
 opt.text = "CALLCODEEcrecover0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover0_0input"
 opt.text = "CALLCODEEcrecover0_0input"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover0_Gas2999"
 opt.text = "CALLCODEEcrecover0_Gas2999"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover0_NoGas"
 opt.text = "CALLCODEEcrecover0_NoGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover0_completeReturnValue"
 opt.text = "CALLCODEEcrecover0_completeReturnValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover0_gas3000"
 opt.text = "CALLCODEEcrecover0_gas3000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover0_overlappingInputOutput"
 opt.text = "CALLCODEEcrecover0_overlappingInputOutput"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover1"
 opt.text = "CALLCODEEcrecover1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover2"
 opt.text = "CALLCODEEcrecover2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover3"
 opt.text = "CALLCODEEcrecover3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecover80"
 opt.text = "CALLCODEEcrecover80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecoverH_prefixed0"
 opt.text = "CALLCODEEcrecoverH_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecoverR_prefixed0"
 opt.text = "CALLCODEEcrecoverR_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecoverS_prefixed0"
 opt.text = "CALLCODEEcrecoverS_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecoverV_prefixed0"
 opt.text = "CALLCODEEcrecoverV_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEEcrecoverV_prefixedf0"
 opt.text = "CALLCODEEcrecoverV_prefixedf0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentitiy_0"
 opt.text = "CALLCODEIdentitiy_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentitiy_1"
 opt.text = "CALLCODEIdentitiy_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentity_1_nonzeroValue"
 opt.text = "CALLCODEIdentity_1_nonzeroValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentity_2"
 opt.text = "CALLCODEIdentity_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentity_3"
 opt.text = "CALLCODEIdentity_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentity_4"
 opt.text = "CALLCODEIdentity_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentity_4_gas17"
 opt.text = "CALLCODEIdentity_4_gas17"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentity_4_gas18"
 opt.text = "CALLCODEIdentity_4_gas18"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODEIdentity_5"
 opt.text = "CALLCODEIdentity_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_0"
 opt.text = "CALLCODERipemd160_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_1"
 opt.text = "CALLCODERipemd160_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_2"
 opt.text = "CALLCODERipemd160_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_3"
 opt.text = "CALLCODERipemd160_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_3_postfixed0"
 opt.text = "CALLCODERipemd160_3_postfixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_3_prefixed0"
 opt.text = "CALLCODERipemd160_3_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_4"
 opt.text = "CALLCODERipemd160_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_4_gas719"
 opt.text = "CALLCODERipemd160_4_gas719"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODERipemd160_5"
 opt.text = "CALLCODERipemd160_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_0"
 opt.text = "CALLCODESha256_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_1"
 opt.text = "CALLCODESha256_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_1_nonzeroValue"
 opt.text = "CALLCODESha256_1_nonzeroValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_2"
 opt.text = "CALLCODESha256_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_3"
 opt.text = "CALLCODESha256_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_3_postfix0"
 opt.text = "CALLCODESha256_3_postfix0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_3_prefix0"
 opt.text = "CALLCODESha256_3_prefix0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_4"
 opt.text = "CALLCODESha256_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_4_gas99"
 opt.text = "CALLCODESha256_4_gas99"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODESha256_5"
 opt.text = "CALLCODESha256_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover0"
 opt.text = "CallEcrecover0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover0_0input"
 opt.text = "CallEcrecover0_0input"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover0_Gas2999"
 opt.text = "CallEcrecover0_Gas2999"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover0_NoGas"
 opt.text = "CallEcrecover0_NoGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover0_completeReturnValue"
 opt.text = "CallEcrecover0_completeReturnValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover0_gas3000"
 opt.text = "CallEcrecover0_gas3000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover0_overlappingInputOutput"
 opt.text = "CallEcrecover0_overlappingInputOutput"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover1"
 opt.text = "CallEcrecover1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover2"
 opt.text = "CallEcrecover2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover3"
 opt.text = "CallEcrecover3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover80"
 opt.text = "CallEcrecover80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecoverCheckLength"
 opt.text = "CallEcrecoverCheckLength"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecoverCheckLengthWrongV"
 opt.text = "CallEcrecoverCheckLengthWrongV"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecoverH_prefixed0"
 opt.text = "CallEcrecoverH_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecoverInvalidSignature"
 opt.text = "CallEcrecoverInvalidSignature"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecoverR_prefixed0"
 opt.text = "CallEcrecoverR_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecoverS_prefixed0"
 opt.text = "CallEcrecoverS_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecoverUnrecoverableKey"
 opt.text = "CallEcrecoverUnrecoverableKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecoverV_prefixed0"
 opt.text = "CallEcrecoverV_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallEcrecover_Overflow"
 opt.text = "CallEcrecover_Overflow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentitiy_0"
 opt.text = "CallIdentitiy_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentitiy_1"
 opt.text = "CallIdentitiy_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentity_1_nonzeroValue"
 opt.text = "CallIdentity_1_nonzeroValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentity_2"
 opt.text = "CallIdentity_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentity_3"
 opt.text = "CallIdentity_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentity_4"
 opt.text = "CallIdentity_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentity_4_gas17"
 opt.text = "CallIdentity_4_gas17"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentity_4_gas18"
 opt.text = "CallIdentity_4_gas18"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentity_5"
 opt.text = "CallIdentity_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallIdentity_6_inputShorterThanOutput"
 opt.text = "CallIdentity_6_inputShorterThanOutput"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_0"
 opt.text = "CallRipemd160_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_1"
 opt.text = "CallRipemd160_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_2"
 opt.text = "CallRipemd160_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_3"
 opt.text = "CallRipemd160_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_3_postfixed0"
 opt.text = "CallRipemd160_3_postfixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_3_prefixed0"
 opt.text = "CallRipemd160_3_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_4"
 opt.text = "CallRipemd160_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_4_gas719"
 opt.text = "CallRipemd160_4_gas719"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRipemd160_5"
 opt.text = "CallRipemd160_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_0"
 opt.text = "CallSha256_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_1"
 opt.text = "CallSha256_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_1_nonzeroValue"
 opt.text = "CallSha256_1_nonzeroValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_2"
 opt.text = "CallSha256_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_3"
 opt.text = "CallSha256_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_3_postfix0"
 opt.text = "CallSha256_3_postfix0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_3_prefix0"
 opt.text = "CallSha256_3_prefix0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_4"
 opt.text = "CallSha256_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_4_gas99"
 opt.text = "CallSha256_4_gas99"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallSha256_5"
 opt.text = "CallSha256_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecrecoverShortBuff"
 opt.text = "ecrecoverShortBuff"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecrecoverWeirdV"
 opt.text = "ecrecoverWeirdV"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modexpRandomInput"
 opt.text = "modexpRandomInput"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modexp_0_0_0_20500"
 opt.text = "modexp_0_0_0_20500"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modexp_0_0_0_22000"
 opt.text = "modexp_0_0_0_22000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modexp_0_0_0_25000"
 opt.text = "modexp_0_0_0_25000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modexp_0_0_0_35000"
 opt.text = "modexp_0_0_0_35000"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stZeroCallsTest") { 
 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALLCODE"
 opt.text = "ZeroValue_CALLCODE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALLCODE_ToEmpty"
 opt.text = "ZeroValue_CALLCODE_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALLCODE_ToNonZeroBalance"
 opt.text = "ZeroValue_CALLCODE_ToNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALLCODE_ToOneStorageKey"
 opt.text = "ZeroValue_CALLCODE_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALL"
 opt.text = "ZeroValue_CALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALL_ToEmpty"
 opt.text = "ZeroValue_CALL_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALL_ToNonZeroBalance"
 opt.text = "ZeroValue_CALL_ToNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_CALL_ToOneStorageKey"
 opt.text = "ZeroValue_CALL_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_DELEGATECALL"
 opt.text = "ZeroValue_DELEGATECALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_DELEGATECALL_ToEmpty"
 opt.text = "ZeroValue_DELEGATECALL_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_DELEGATECALL_ToNonZeroBalance"
 opt.text = "ZeroValue_DELEGATECALL_ToNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_DELEGATECALL_ToOneStorageKey"
 opt.text = "ZeroValue_DELEGATECALL_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_SUICIDE"
 opt.text = "ZeroValue_SUICIDE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_SUICIDE_ToEmpty"
 opt.text = "ZeroValue_SUICIDE_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_SUICIDE_ToNonZeroBalance"
 opt.text = "ZeroValue_SUICIDE_ToNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_SUICIDE_ToOneStorageKey"
 opt.text = "ZeroValue_SUICIDE_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_TransactionCALL"
 opt.text = "ZeroValue_TransactionCALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_TransactionCALL_ToEmpty"
 opt.text = "ZeroValue_TransactionCALL_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_TransactionCALL_ToNonZeroBalance"
 opt.text = "ZeroValue_TransactionCALL_ToNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_TransactionCALL_ToOneStorageKey"
 opt.text = "ZeroValue_TransactionCALL_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_TransactionCALLwithData"
 opt.text = "ZeroValue_TransactionCALLwithData"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_TransactionCALLwithData_ToEmpty"
 opt.text = "ZeroValue_TransactionCALLwithData_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_TransactionCALLwithData_ToNonZeroBalance"
 opt.text = "ZeroValue_TransactionCALLwithData_ToNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ZeroValue_TransactionCALLwithData_ToOneStorageKey"
 opt.text = "ZeroValue_TransactionCALLwithData_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stSStoreTest") { 
 var opt = document.createElement("option")
 opt.value = "InitCollision"
 opt.text = "InitCollision"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "InitCollisionNonZeroNonce"
 opt.text = "InitCollisionNonZeroNonce"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SstoreCallToSelfSubRefundBelowZero"
 opt.text = "SstoreCallToSelfSubRefundBelowZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstoreGas"
 opt.text = "sstoreGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_0to0"
 opt.text = "sstore_0to0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_0to0to0"
 opt.text = "sstore_0to0to0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_0to0toX"
 opt.text = "sstore_0to0toX"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_0toX"
 opt.text = "sstore_0toX"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_0toXto0"
 opt.text = "sstore_0toXto0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_0toXto0toX"
 opt.text = "sstore_0toXto0toX"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_0toXtoX"
 opt.text = "sstore_0toXtoX"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_0toXtoY"
 opt.text = "sstore_0toXtoY"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_Xto0"
 opt.text = "sstore_Xto0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_Xto0to0"
 opt.text = "sstore_Xto0to0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_Xto0toX"
 opt.text = "sstore_Xto0toX"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_Xto0toXto0"
 opt.text = "sstore_Xto0toXto0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_Xto0toY"
 opt.text = "sstore_Xto0toY"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoX"
 opt.text = "sstore_XtoX"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoXto0"
 opt.text = "sstore_XtoXto0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoXtoX"
 opt.text = "sstore_XtoXtoX"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoXtoY"
 opt.text = "sstore_XtoXtoY"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoY"
 opt.text = "sstore_XtoY"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoYto0"
 opt.text = "sstore_XtoYto0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoYtoX"
 opt.text = "sstore_XtoYtoX"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoYtoY"
 opt.text = "sstore_XtoYtoY"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_XtoYtoZ"
 opt.text = "sstore_XtoYtoZ"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_changeFromExternalCallInInitCode"
 opt.text = "sstore_changeFromExternalCallInInitCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_gasLeft"
 opt.text = "sstore_gasLeft"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stLogTests") { 
 var opt = document.createElement("option")
 opt.value = "log0_emptyMem"
 opt.text = "log0_emptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log0_logMemStartTooHigh"
 opt.text = "log0_logMemStartTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log0_logMemsizeTooHigh"
 opt.text = "log0_logMemsizeTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log0_logMemsizeZero"
 opt.text = "log0_logMemsizeZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log0_nonEmptyMem"
 opt.text = "log0_nonEmptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log0_nonEmptyMem_logMemSize1"
 opt.text = "log0_nonEmptyMem_logMemSize1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log0_nonEmptyMem_logMemSize1_logMemStart31"
 opt.text = "log0_nonEmptyMem_logMemSize1_logMemStart31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_Caller"
 opt.text = "log1_Caller"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_MaxTopic"
 opt.text = "log1_MaxTopic"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_emptyMem"
 opt.text = "log1_emptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_logMemStartTooHigh"
 opt.text = "log1_logMemStartTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_logMemsizeTooHigh"
 opt.text = "log1_logMemsizeTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_logMemsizeZero"
 opt.text = "log1_logMemsizeZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_nonEmptyMem"
 opt.text = "log1_nonEmptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_nonEmptyMem_logMemSize1"
 opt.text = "log1_nonEmptyMem_logMemSize1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_nonEmptyMem_logMemSize1_logMemStart31"
 opt.text = "log1_nonEmptyMem_logMemSize1_logMemStart31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_Caller"
 opt.text = "log2_Caller"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_MaxTopic"
 opt.text = "log2_MaxTopic"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_emptyMem"
 opt.text = "log2_emptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_logMemStartTooHigh"
 opt.text = "log2_logMemStartTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_logMemsizeTooHigh"
 opt.text = "log2_logMemsizeTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_logMemsizeZero"
 opt.text = "log2_logMemsizeZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_nonEmptyMem"
 opt.text = "log2_nonEmptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_nonEmptyMem_logMemSize1"
 opt.text = "log2_nonEmptyMem_logMemSize1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_nonEmptyMem_logMemSize1_logMemStart31"
 opt.text = "log2_nonEmptyMem_logMemSize1_logMemStart31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_Caller"
 opt.text = "log3_Caller"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_MaxTopic"
 opt.text = "log3_MaxTopic"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_PC"
 opt.text = "log3_PC"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_emptyMem"
 opt.text = "log3_emptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_logMemStartTooHigh"
 opt.text = "log3_logMemStartTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_logMemsizeTooHigh"
 opt.text = "log3_logMemsizeTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_logMemsizeZero"
 opt.text = "log3_logMemsizeZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_nonEmptyMem"
 opt.text = "log3_nonEmptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_nonEmptyMem_logMemSize1"
 opt.text = "log3_nonEmptyMem_logMemSize1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_nonEmptyMem_logMemSize1_logMemStart31"
 opt.text = "log3_nonEmptyMem_logMemSize1_logMemStart31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_Caller"
 opt.text = "log4_Caller"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_MaxTopic"
 opt.text = "log4_MaxTopic"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_PC"
 opt.text = "log4_PC"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_emptyMem"
 opt.text = "log4_emptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_logMemStartTooHigh"
 opt.text = "log4_logMemStartTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_logMemsizeTooHigh"
 opt.text = "log4_logMemsizeTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_logMemsizeZero"
 opt.text = "log4_logMemsizeZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_nonEmptyMem"
 opt.text = "log4_nonEmptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_nonEmptyMem_logMemSize1"
 opt.text = "log4_nonEmptyMem_logMemSize1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_nonEmptyMem_logMemSize1_logMemStart31"
 opt.text = "log4_nonEmptyMem_logMemSize1_logMemStart31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "logInOOG_Call"
 opt.text = "logInOOG_Call"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stStackTests") { 
 var opt = document.createElement("option")
 opt.value = "shallowStack"
 opt.text = "shallowStack"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackOverflowDUP"
 opt.text = "stackOverflowDUP"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackOverflow"
 opt.text = "stackOverflow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackOverflowM1DUP"
 opt.text = "stackOverflowM1DUP"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackOverflowM1"
 opt.text = "stackOverflowM1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackOverflowM1PUSH"
 opt.text = "stackOverflowM1PUSH"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackOverflowPUSH"
 opt.text = "stackOverflowPUSH"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackOverflowSWAP"
 opt.text = "stackOverflowSWAP"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stacksanitySWAP"
 opt.text = "stacksanitySWAP"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "underflowTest"
 opt.text = "underflowTest"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stTransitionTest") { 
 var opt = document.createElement("option")
 opt.value = "createNameRegistratorPerTxsAfter"
 opt.text = "createNameRegistratorPerTxsAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorPerTxsAt"
 opt.text = "createNameRegistratorPerTxsAt"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorPerTxsBefore"
 opt.text = "createNameRegistratorPerTxsBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallAfterTransition"
 opt.text = "delegatecallAfterTransition"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallAtTransition"
 opt.text = "delegatecallAtTransition"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallBeforeTransition"
 opt.text = "delegatecallBeforeTransition"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stZeroKnowledge") { 
 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_2_28000_128"
 opt.text = "ecmul_1-2_2_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_2_28000_96"
 opt.text = "ecmul_1-2_2_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_340282366920938463463374607431768211456_21000_128"
 opt.text = "ecmul_1-2_340282366920938463463374607431768211456_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_340282366920938463463374607431768211456_21000_80"
 opt.text = "ecmul_1-2_340282366920938463463374607431768211456_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_340282366920938463463374607431768211456_21000_96"
 opt.text = "ecmul_1-2_340282366920938463463374607431768211456_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_340282366920938463463374607431768211456_28000_128"
 opt.text = "ecmul_1-2_340282366920938463463374607431768211456_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_340282366920938463463374607431768211456_28000_80"
 opt.text = "ecmul_1-2_340282366920938463463374607431768211456_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_340282366920938463463374607431768211456_28000_96"
 opt.text = "ecmul_1-2_340282366920938463463374607431768211456_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_5616_21000_128"
 opt.text = "ecmul_1-2_5616_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_5616_21000_96"
 opt.text = "ecmul_1-2_5616_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_5616_28000_128"
 opt.text = "ecmul_1-2_5616_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_5617_21000_128"
 opt.text = "ecmul_1-2_5617_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_5617_21000_96"
 opt.text = "ecmul_1-2_5617_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_5617_28000_128"
 opt.text = "ecmul_1-2_5617_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_5617_28000_96"
 opt.text = "ecmul_1-2_5617_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_616_28000_96"
 opt.text = "ecmul_1-2_616_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_9935_21000_128"
 opt.text = "ecmul_1-2_9935_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_9935_21000_96"
 opt.text = "ecmul_1-2_9935_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_9935_28000_128"
 opt.text = "ecmul_1-2_9935_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_9935_28000_96"
 opt.text = "ecmul_1-2_9935_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_9_21000_128"
 opt.text = "ecmul_1-2_9_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_9_21000_96"
 opt.text = "ecmul_1-2_9_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_9_28000_128"
 opt.text = "ecmul_1-2_9_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_9_28000_96"
 opt.text = "ecmul_1-2_9_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_0_21000_128"
 opt.text = "ecmul_1-3_0_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_0_21000_64"
 opt.text = "ecmul_1-3_0_21000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_0_21000_80"
 opt.text = "ecmul_1-3_0_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_0_21000_96"
 opt.text = "ecmul_1-3_0_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_0_28000_128"
 opt.text = "ecmul_1-3_0_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_0_28000_64"
 opt.text = "ecmul_1-3_0_28000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_0_28000_80"
 opt.text = "ecmul_1-3_0_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_0_28000_96"
 opt.text = "ecmul_1-3_0_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_1_21000_128"
 opt.text = "ecmul_1-3_1_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_1_21000_96"
 opt.text = "ecmul_1-3_1_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_1_28000_128"
 opt.text = "ecmul_1-3_1_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_1_28000_96"
 opt.text = "ecmul_1-3_1_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_2_21000_128"
 opt.text = "ecmul_1-3_2_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_2_21000_96"
 opt.text = "ecmul_1-3_2_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_2_28000_128"
 opt.text = "ecmul_1-3_2_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_2_28000_96"
 opt.text = "ecmul_1-3_2_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_340282366920938463463374607431768211456_21000_128"
 opt.text = "ecmul_1-3_340282366920938463463374607431768211456_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_340282366920938463463374607431768211456_21000_80"
 opt.text = "ecmul_1-3_340282366920938463463374607431768211456_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_340282366920938463463374607431768211456_21000_96"
 opt.text = "ecmul_1-3_340282366920938463463374607431768211456_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_340282366920938463463374607431768211456_28000_128"
 opt.text = "ecmul_1-3_340282366920938463463374607431768211456_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_340282366920938463463374607431768211456_28000_80"
 opt.text = "ecmul_1-3_340282366920938463463374607431768211456_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_340282366920938463463374607431768211456_28000_96"
 opt.text = "ecmul_1-3_340282366920938463463374607431768211456_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_5616_21000_128"
 opt.text = "ecmul_1-3_5616_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_5616_21000_96"
 opt.text = "ecmul_1-3_5616_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_5616_28000_128"
 opt.text = "ecmul_1-3_5616_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_5616_28000_96"
 opt.text = "ecmul_1-3_5616_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_5617_21000_128"
 opt.text = "ecmul_1-3_5617_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_5617_21000_96"
 opt.text = "ecmul_1-3_5617_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_5617_28000_128"
 opt.text = "ecmul_1-3_5617_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_5617_28000_96"
 opt.text = "ecmul_1-3_5617_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_9935_21000_128"
 opt.text = "ecmul_1-3_9935_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_9935_21000_96"
 opt.text = "ecmul_1-3_9935_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_9935_28000_128"
 opt.text = "ecmul_1-3_9935_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_9935_28000_96"
 opt.text = "ecmul_1-3_9935_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_9_21000_128"
 opt.text = "ecmul_1-3_9_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_9_21000_96"
 opt.text = "ecmul_1-3_9_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_9_28000_128"
 opt.text = "ecmul_1-3_9_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-3_9_28000_96"
 opt.text = "ecmul_1-3_9_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_0_21000_128"
 opt.text = "ecmul_7827-6598_0_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_0_21000_64"
 opt.text = "ecmul_7827-6598_0_21000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_0_21000_80"
 opt.text = "ecmul_7827-6598_0_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_0_21000_96"
 opt.text = "ecmul_7827-6598_0_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_0_28000_128"
 opt.text = "ecmul_7827-6598_0_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_0_28000_64"
 opt.text = "ecmul_7827-6598_0_28000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_0_28000_80"
 opt.text = "ecmul_7827-6598_0_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_0_28000_96"
 opt.text = "ecmul_7827-6598_0_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1456_21000_128"
 opt.text = "ecmul_7827-6598_1456_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1456_21000_80"
 opt.text = "ecmul_7827-6598_1456_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1456_21000_96"
 opt.text = "ecmul_7827-6598_1456_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1456_28000_128"
 opt.text = "ecmul_7827-6598_1456_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1456_28000_80"
 opt.text = "ecmul_7827-6598_1456_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1456_28000_96"
 opt.text = "ecmul_7827-6598_1456_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1_21000_128"
 opt.text = "ecmul_7827-6598_1_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1_21000_96"
 opt.text = "ecmul_7827-6598_1_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1_28000_128"
 opt.text = "ecmul_7827-6598_1_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_1_28000_96"
 opt.text = "ecmul_7827-6598_1_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_2_21000_128"
 opt.text = "ecmul_7827-6598_2_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_2_21000_96"
 opt.text = "ecmul_7827-6598_2_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_2_28000_128"
 opt.text = "ecmul_7827-6598_2_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_2_28000_96"
 opt.text = "ecmul_7827-6598_2_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_5616_21000_128"
 opt.text = "ecmul_7827-6598_5616_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_5616_21000_96"
 opt.text = "ecmul_7827-6598_5616_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_5616_28000_128"
 opt.text = "ecmul_7827-6598_5616_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_5616_28000_96"
 opt.text = "ecmul_7827-6598_5616_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_5617_21000_128"
 opt.text = "ecmul_7827-6598_5617_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_5617_21000_96"
 opt.text = "ecmul_7827-6598_5617_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_5617_28000_128"
 opt.text = "ecmul_7827-6598_5617_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_5617_28000_96"
 opt.text = "ecmul_7827-6598_5617_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_9935_21000_128"
 opt.text = "ecmul_7827-6598_9935_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_9935_21000_96"
 opt.text = "ecmul_7827-6598_9935_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_9935_28000_128"
 opt.text = "ecmul_7827-6598_9935_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_9935_28000_96"
 opt.text = "ecmul_7827-6598_9935_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_9_21000_128"
 opt.text = "ecmul_7827-6598_9_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_9_21000_96"
 opt.text = "ecmul_7827-6598_9_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_9_28000_128"
 opt.text = "ecmul_7827-6598_9_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_7827-6598_9_28000_96"
 opt.text = "ecmul_7827-6598_9_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_bad_length_191"
 opt.text = "ecpairing_bad_length_191"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_bad_length_193"
 opt.text = "ecpairing_bad_length_193"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_empty_data"
 opt.text = "ecpairing_empty_data"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_empty_data_insufficient_gas"
 opt.text = "ecpairing_empty_data_insufficient_gas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_one_point_fail"
 opt.text = "ecpairing_one_point_fail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_one_point_insufficient_gas"
 opt.text = "ecpairing_one_point_insufficient_gas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_one_point_not_in_subgroup"
 opt.text = "ecpairing_one_point_not_in_subgroup"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_one_point_with_g1_zero"
 opt.text = "ecpairing_one_point_with_g1_zero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_one_point_with_g2_zero"
 opt.text = "ecpairing_one_point_with_g2_zero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_one_point_with_g2_zero_and_g1_invalid"
 opt.text = "ecpairing_one_point_with_g2_zero_and_g1_invalid"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_perturb_g2_by_curve_order"
 opt.text = "ecpairing_perturb_g2_by_curve_order"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_perturb_g2_by_field_modulus"
 opt.text = "ecpairing_perturb_g2_by_field_modulus"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_perturb_g2_by_field_modulus_again"
 opt.text = "ecpairing_perturb_g2_by_field_modulus_again"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_perturb_g2_by_one"
 opt.text = "ecpairing_perturb_g2_by_one"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_perturb_zeropoint_by_curve_order"
 opt.text = "ecpairing_perturb_zeropoint_by_curve_order"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_perturb_zeropoint_by_field_modulus"
 opt.text = "ecpairing_perturb_zeropoint_by_field_modulus"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_perturb_zeropoint_by_one"
 opt.text = "ecpairing_perturb_zeropoint_by_one"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_three_point_fail_1"
 opt.text = "ecpairing_three_point_fail_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_three_point_match_1"
 opt.text = "ecpairing_three_point_match_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_point_fail_1"
 opt.text = "ecpairing_two_point_fail_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_point_fail_2"
 opt.text = "ecpairing_two_point_fail_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_point_match_1"
 opt.text = "ecpairing_two_point_match_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_point_match_2"
 opt.text = "ecpairing_two_point_match_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_point_match_3"
 opt.text = "ecpairing_two_point_match_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_point_match_4"
 opt.text = "ecpairing_two_point_match_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_point_match_5"
 opt.text = "ecpairing_two_point_match_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_point_oog"
 opt.text = "ecpairing_two_point_oog"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecpairing_two_points_with_one_g2_zero"
 opt.text = "ecpairing_two_points_with_one_g2_zero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "pairingTest"
 opt.text = "pairingTest"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "pointAdd"
 opt.text = "pointAdd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "pointAddTrunc"
 opt.text = "pointAddTrunc"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "pointMulAdd2"
 opt.text = "pointMulAdd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "pointMulAdd"
 opt.text = "pointMulAdd"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stCallDelegateCodesHomestead") { 
 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001"
 opt.text = "callcallcallcode_001"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGE"
 opt.text = "callcallcallcode_001_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGMAfter"
 opt.text = "callcallcallcode_001_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGMBefore"
 opt.text = "callcallcallcode_001_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_SuicideEnd"
 opt.text = "callcallcallcode_001_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_SuicideMiddle"
 opt.text = "callcallcallcode_001_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_ABCB_RECURSIVE"
 opt.text = "callcallcallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01"
 opt.text = "callcallcode_01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01_OOGE"
 opt.text = "callcallcode_01_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01_SuicideEnd"
 opt.text = "callcallcode_01_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010"
 opt.text = "callcallcodecall_010"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGE"
 opt.text = "callcallcodecall_010_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGMAfter"
 opt.text = "callcallcodecall_010_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGMBefore"
 opt.text = "callcallcodecall_010_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_SuicideEnd"
 opt.text = "callcallcodecall_010_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_SuicideMiddle"
 opt.text = "callcallcodecall_010_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_ABCB_RECURSIVE"
 opt.text = "callcallcodecall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011"
 opt.text = "callcallcodecallcode_011"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGE"
 opt.text = "callcallcodecallcode_011_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGMAfter"
 opt.text = "callcallcodecallcode_011_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGMBefore"
 opt.text = "callcallcodecallcode_011_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_SuicideEnd"
 opt.text = "callcallcodecallcode_011_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_SuicideMiddle"
 opt.text = "callcallcodecallcode_011_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_ABCB_RECURSIVE"
 opt.text = "callcallcodecallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10"
 opt.text = "callcodecall_10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10_OOGE"
 opt.text = "callcodecall_10_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10_SuicideEnd"
 opt.text = "callcodecall_10_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100"
 opt.text = "callcodecallcall_100"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGE"
 opt.text = "callcodecallcall_100_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGMAfter"
 opt.text = "callcodecallcall_100_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGMBefore"
 opt.text = "callcodecallcall_100_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_SuicideEnd"
 opt.text = "callcodecallcall_100_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_SuicideMiddle"
 opt.text = "callcodecallcall_100_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_ABCB_RECURSIVE"
 opt.text = "callcodecallcall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101"
 opt.text = "callcodecallcallcode_101"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGE"
 opt.text = "callcodecallcallcode_101_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGMAfter"
 opt.text = "callcodecallcallcode_101_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGMBefore"
 opt.text = "callcodecallcallcode_101_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_SuicideEnd"
 opt.text = "callcodecallcallcode_101_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_SuicideMiddle"
 opt.text = "callcodecallcallcode_101_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_ABCB_RECURSIVE"
 opt.text = "callcodecallcallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11"
 opt.text = "callcodecallcode_11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11_OOGE"
 opt.text = "callcodecallcode_11_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11_SuicideEnd"
 opt.text = "callcodecallcode_11_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110"
 opt.text = "callcodecallcodecall_110"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGE"
 opt.text = "callcodecallcodecall_110_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGMAfter"
 opt.text = "callcodecallcodecall_110_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGMBefore"
 opt.text = "callcodecallcodecall_110_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_SuicideEnd"
 opt.text = "callcodecallcodecall_110_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_SuicideMiddle"
 opt.text = "callcodecallcodecall_110_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_ABCB_RECURSIVE"
 opt.text = "callcodecallcodecall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111"
 opt.text = "callcodecallcodecallcode_111"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGE"
 opt.text = "callcodecallcodecallcode_111_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGMAfter"
 opt.text = "callcodecallcodecallcode_111_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGMBefore"
 opt.text = "callcodecallcodecallcode_111_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_SuicideEnd"
 opt.text = "callcodecallcodecallcode_111_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_SuicideMiddle"
 opt.text = "callcodecallcodecallcode_111_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_ABCB_RECURSIVE"
 opt.text = "callcodecallcodecallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stHomesteadSpecific") { 
 var opt = document.createElement("option")
 opt.value = "contractCreationOOGdontLeaveEmptyContract"
 opt.text = "contractCreationOOGdontLeaveEmptyContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "contractCreationOOGdontLeaveEmptyContractViaTransaction"
 opt.text = "contractCreationOOGdontLeaveEmptyContractViaTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createContractViaContract"
 opt.text = "createContractViaContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createContractViaContractOOGInitCode"
 opt.text = "createContractViaContractOOGInitCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createContractViaTransactionCost53000"
 opt.text = "createContractViaTransactionCost53000"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stExample") { 
 var opt = document.createElement("option")
 opt.value = "accessListExample"
 opt.text = "accessListExample"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "add11"
 opt.text = "add11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "add11_yml"
 opt.text = "add11_yml"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "basefeeExample"
 opt.text = "basefeeExample"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "eip1559"
 opt.text = "eip1559"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "indexesOmitExample"
 opt.text = "indexesOmitExample"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "invalidTr"
 opt.text = "invalidTr"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "labelsExample"
 opt.text = "labelsExample"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mergeTest"
 opt.text = "mergeTest"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "rangesExample"
 opt.text = "rangesExample"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "solidityExample"
 opt.text = "solidityExample"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "yulExample"
 opt.text = "yulExample"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stSolidityTest") { 
 var opt = document.createElement("option")
 opt.value = "AmbiguousMethod"
 opt.text = "AmbiguousMethod"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ByZero"
 opt.text = "ByZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallInfiniteLoop"
 opt.text = "CallInfiniteLoop"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallLowLevelCreatesSolidity"
 opt.text = "CallLowLevelCreatesSolidity"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveMethods"
 opt.text = "CallRecursiveMethods"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ContractInheritance"
 opt.text = "ContractInheritance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateContractFromMethod"
 opt.text = "CreateContractFromMethod"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RecursiveCreateContractsCreate4Contracts"
 opt.text = "RecursiveCreateContractsCreate4Contracts"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RecursiveCreateContracts"
 opt.text = "RecursiveCreateContracts"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SelfDestruct"
 opt.text = "SelfDestruct"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestBlockAndTransactionProperties"
 opt.text = "TestBlockAndTransactionProperties"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestContractInteraction"
 opt.text = "TestContractInteraction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestContractSuicide"
 opt.text = "TestContractSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestCryptographicFunctions"
 opt.text = "TestCryptographicFunctions"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestKeywords"
 opt.text = "TestKeywords"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestOverflow"
 opt.text = "TestOverflow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestStoreGasPrices"
 opt.text = "TestStoreGasPrices"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestStructuresAndVariabless"
 opt.text = "TestStructuresAndVariabless"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stMemExpandingEIP150Calls") { 
 var opt = document.createElement("option")
 opt.value = "CallAndCallcodeConsumeMoreGasThenTransactionHasWithMemExpandingCalls"
 opt.text = "CallAndCallcodeConsumeMoreGasThenTransactionHasWithMemExpandingCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallAskMoreGasOnDepth2ThenTransactionHasWithMemExpandingCalls"
 opt.text = "CallAskMoreGasOnDepth2ThenTransactionHasWithMemExpandingCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallGoesOOGOnSecondLevel2WithMemExpandingCalls"
 opt.text = "CallGoesOOGOnSecondLevel2WithMemExpandingCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallGoesOOGOnSecondLevelWithMemExpandingCalls"
 opt.text = "CallGoesOOGOnSecondLevelWithMemExpandingCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateAndGasInsideCreateWithMemExpandingCalls"
 opt.text = "CreateAndGasInsideCreateWithMemExpandingCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DelegateCallOnEIPWithMemExpandingCalls"
 opt.text = "DelegateCallOnEIPWithMemExpandingCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ExecuteCallThatAskMoreGasThenTransactionHasWithMemExpandingCalls"
 opt.text = "ExecuteCallThatAskMoreGasThenTransactionHasWithMemExpandingCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NewGasPriceForCodesWithMemExpandingCalls"
 opt.text = "NewGasPriceForCodesWithMemExpandingCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "OOGinReturn"
 opt.text = "OOGinReturn"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stMemoryStressTest") { 
 var opt = document.createElement("option")
 opt.value = "CALLCODE_Bounds2"
 opt.text = "CALLCODE_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODE_Bounds3"
 opt.text = "CALLCODE_Bounds3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODE_Bounds4"
 opt.text = "CALLCODE_Bounds4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALLCODE_Bounds"
 opt.text = "CALLCODE_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALL_Bounds2"
 opt.text = "CALL_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALL_Bounds2a"
 opt.text = "CALL_Bounds2a"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALL_Bounds3"
 opt.text = "CALL_Bounds3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALL_Bounds"
 opt.text = "CALL_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_Bounds2"
 opt.text = "CREATE_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_Bounds3"
 opt.text = "CREATE_Bounds3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_Bounds"
 opt.text = "CREATE_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DELEGATECALL_Bounds2"
 opt.text = "DELEGATECALL_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DELEGATECALL_Bounds3"
 opt.text = "DELEGATECALL_Bounds3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DELEGATECALL_Bounds"
 opt.text = "DELEGATECALL_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DUP_Bounds"
 opt.text = "DUP_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "FillStack"
 opt.text = "FillStack"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "JUMPI_Bounds"
 opt.text = "JUMPI_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "JUMP_Bounds2"
 opt.text = "JUMP_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "JUMP_Bounds"
 opt.text = "JUMP_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "MLOAD_Bounds2"
 opt.text = "MLOAD_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "MLOAD_Bounds3"
 opt.text = "MLOAD_Bounds3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "MLOAD_Bounds"
 opt.text = "MLOAD_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "MSTORE_Bounds2"
 opt.text = "MSTORE_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "MSTORE_Bounds2a"
 opt.text = "MSTORE_Bounds2a"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "MSTORE_Bounds"
 opt.text = "MSTORE_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "POP_Bounds"
 opt.text = "POP_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RETURN_Bounds"
 opt.text = "RETURN_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SLOAD_Bounds"
 opt.text = "SLOAD_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SSTORE_Bounds"
 opt.text = "SSTORE_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mload32bitBound2"
 opt.text = "mload32bitBound2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mload32bitBound"
 opt.text = "mload32bitBound"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mload32bitBound_Msize"
 opt.text = "mload32bitBound_Msize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mload32bitBound_return2"
 opt.text = "mload32bitBound_return2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mload32bitBound_return"
 opt.text = "mload32bitBound_return"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CALL_Bounds2"
 opt.text = "static_CALL_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CALL_Bounds2a"
 opt.text = "static_CALL_Bounds2a"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CALL_Bounds3"
 opt.text = "static_CALL_Bounds3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CALL_Bounds"
 opt.text = "static_CALL_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stArgsZeroOneBalance") { 
 var opt = document.createElement("option")
 opt.value = "addNonConst"
 opt.text = "addNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "addmodNonConst"
 opt.text = "addmodNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "andNonConst"
 opt.text = "andNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "balanceNonConst"
 opt.text = "balanceNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "byteNonConst"
 opt.text = "byteNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callNonConst"
 opt.text = "callNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeNonConst"
 opt.text = "callcodeNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "calldatacopyNonConst"
 opt.text = "calldatacopyNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "calldataloadNonConst"
 opt.text = "calldataloadNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "codecopyNonConst"
 opt.text = "codecopyNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNonConst"
 opt.text = "createNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecallNonConst"
 opt.text = "delegatecallNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "divNonConst"
 opt.text = "divNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "eqNonConst"
 opt.text = "eqNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "expNonConst"
 opt.text = "expNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extcodecopyNonConst"
 opt.text = "extcodecopyNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extcodesizeNonConst"
 opt.text = "extcodesizeNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gtNonConst"
 opt.text = "gtNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "iszeroNonConst"
 opt.text = "iszeroNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "jumpNonConst"
 opt.text = "jumpNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "jumpiNonConst"
 opt.text = "jumpiNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log0NonConst"
 opt.text = "log0NonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1NonConst"
 opt.text = "log1NonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2NonConst"
 opt.text = "log2NonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3NonConst"
 opt.text = "log3NonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ltNonConst"
 opt.text = "ltNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mloadNonConst"
 opt.text = "mloadNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modNonConst"
 opt.text = "modNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mstore8NonConst"
 opt.text = "mstore8NonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mstoreNonConst"
 opt.text = "mstoreNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mulNonConst"
 opt.text = "mulNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mulmodNonConst"
 opt.text = "mulmodNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "notNonConst"
 opt.text = "notNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "orNonConst"
 opt.text = "orNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returnNonConst"
 opt.text = "returnNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sdivNonConst"
 opt.text = "sdivNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sgtNonConst"
 opt.text = "sgtNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sha3NonConst"
 opt.text = "sha3NonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "signextNonConst"
 opt.text = "signextNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sloadNonConst"
 opt.text = "sloadNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sltNonConst"
 opt.text = "sltNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "smodNonConst"
 opt.text = "smodNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstoreNonConst"
 opt.text = "sstoreNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "subNonConst"
 opt.text = "subNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideNonConst"
 opt.text = "suicideNonConst"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "xorNonConst"
 opt.text = "xorNonConst"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stCreateTest") { 
 var opt = document.createElement("option")
 opt.value = "CREATE_AcreateB_BSuicide_BStore"
 opt.text = "CREATE_AcreateB_BSuicide_BStore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_ContractRETURNBigOffset"
 opt.text = "CREATE_ContractRETURNBigOffset"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_ContractSSTOREDuringInit"
 opt.text = "CREATE_ContractSSTOREDuringInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_ContractSuicideDuringInit"
 opt.text = "CREATE_ContractSuicideDuringInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_ContractSuicideDuringInit_ThenStoreThenReturn"
 opt.text = "CREATE_ContractSuicideDuringInit_ThenStoreThenReturn"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_ContractSuicideDuringInit_WithValue"
 opt.text = "CREATE_ContractSuicideDuringInit_WithValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_ContractSuicideDuringInit_WithValueToItself"
 opt.text = "CREATE_ContractSuicideDuringInit_WithValueToItself"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EContractCreateEContractInInit_Tr"
 opt.text = "CREATE_EContractCreateEContractInInit_Tr"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EContractCreateNEContractInInitOOG_Tr"
 opt.text = "CREATE_EContractCreateNEContractInInitOOG_Tr"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EContractCreateNEContractInInit_Tr"
 opt.text = "CREATE_EContractCreateNEContractInInit_Tr"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EContract_ThenCALLToNonExistentAcc"
 opt.text = "CREATE_EContract_ThenCALLToNonExistentAcc"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EOF1"
 opt.text = "CREATE_EOF1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EmptyContractAndCallIt_0wei"
 opt.text = "CREATE_EmptyContractAndCallIt_0wei"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EmptyContractAndCallIt_1wei"
 opt.text = "CREATE_EmptyContractAndCallIt_1wei"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EmptyContract"
 opt.text = "CREATE_EmptyContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EmptyContractWithBalance"
 opt.text = "CREATE_EmptyContractWithBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EmptyContractWithStorageAndCallIt_0wei"
 opt.text = "CREATE_EmptyContractWithStorageAndCallIt_0wei"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EmptyContractWithStorageAndCallIt_1wei"
 opt.text = "CREATE_EmptyContractWithStorageAndCallIt_1wei"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_EmptyContractWithStorage"
 opt.text = "CREATE_EmptyContractWithStorage"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_FirstByte_loop"
 opt.text = "CREATE_FirstByte_loop"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_HighNonce"
 opt.text = "CREATE_HighNonce"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_HighNonceMinus1"
 opt.text = "CREATE_HighNonceMinus1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE_empty000CreateinInitCode_Transaction"
 opt.text = "CREATE_empty000CreateinInitCode_Transaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CodeInConstructor"
 opt.text = "CodeInConstructor"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateAddressWarmAfterFail"
 opt.text = "CreateAddressWarmAfterFail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateCollisionResults"
 opt.text = "CreateCollisionResults"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateCollisionToEmpty"
 opt.text = "CreateCollisionToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGFromCallRefunds"
 opt.text = "CreateOOGFromCallRefunds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGFromEOARefunds"
 opt.text = "CreateOOGFromEOARefunds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGafterInitCode"
 opt.text = "CreateOOGafterInitCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGafterInitCodeReturndata2"
 opt.text = "CreateOOGafterInitCodeReturndata2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGafterInitCodeReturndata3"
 opt.text = "CreateOOGafterInitCodeReturndata3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGafterInitCodeReturndata"
 opt.text = "CreateOOGafterInitCodeReturndata"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGafterInitCodeReturndataSize"
 opt.text = "CreateOOGafterInitCodeReturndataSize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGafterInitCodeRevert2"
 opt.text = "CreateOOGafterInitCodeRevert2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGafterInitCodeRevert"
 opt.text = "CreateOOGafterInitCodeRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateOOGafterMaxCodesize"
 opt.text = "CreateOOGafterMaxCodesize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateResults"
 opt.text = "CreateResults"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateTransactionHighNonce"
 opt.text = "CreateTransactionHighNonce"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionCollisionToEmptyButCode"
 opt.text = "TransactionCollisionToEmptyButCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionCollisionToEmptyButNonce"
 opt.text = "TransactionCollisionToEmptyButNonce"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionCollisionToEmpty"
 opt.text = "TransactionCollisionToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createFailResult"
 opt.text = "createFailResult"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createLargeResult"
 opt.text = "createLargeResult"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stAttackTest") { 
 var opt = document.createElement("option")
 opt.value = "ContractCreationSpam"
 opt.text = "ContractCreationSpam"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CrashingTransaction"
 opt.text = "CrashingTransaction"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stRecursiveCreate") { 
 var opt = document.createElement("option")
 opt.value = "recursiveCreate"
 opt.text = "recursiveCreate"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "recursiveCreateReturnValue"
 opt.text = "recursiveCreateReturnValue"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stShift") { 
 var opt = document.createElement("option")
 opt.value = "sar00"
 opt.text = "sar00"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar01"
 opt.text = "sar01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar10"
 opt.text = "sar10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar11"
 opt.text = "sar11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_0_256-1"
 opt.text = "sar_0_256-1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^254_254"
 opt.text = "sar_2^254_254"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^255-1_248"
 opt.text = "sar_2^255-1_248"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^255-1_254"
 opt.text = "sar_2^255-1_254"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^255-1_255"
 opt.text = "sar_2^255-1_255"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^255-1_256"
 opt.text = "sar_2^255-1_256"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^255_1"
 opt.text = "sar_2^255_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^255_255"
 opt.text = "sar_2^255_255"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^255_256"
 opt.text = "sar_2^255_256"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^255_257"
 opt.text = "sar_2^255_257"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^256-1_0"
 opt.text = "sar_2^256-1_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^256-1_1"
 opt.text = "sar_2^256-1_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^256-1_255"
 opt.text = "sar_2^256-1_255"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sar_2^256-1_256"
 opt.text = "sar_2^256-1_256"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shiftCombinations"
 opt.text = "shiftCombinations"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shiftSignedCombinations"
 opt.text = "shiftSignedCombinations"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl01-0100"
 opt.text = "shl01-0100"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl01-0101"
 opt.text = "shl01-0101"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl01-ff"
 opt.text = "shl01-ff"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl01"
 opt.text = "shl01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl10"
 opt.text = "shl10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl11"
 opt.text = "shl11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl_-1_0"
 opt.text = "shl_-1_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl_-1_1"
 opt.text = "shl_-1_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl_-1_255"
 opt.text = "shl_-1_255"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl_-1_256"
 opt.text = "shl_-1_256"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shl_2^255-1_1"
 opt.text = "shl_2^255-1_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr01"
 opt.text = "shr01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr10"
 opt.text = "shr10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr11"
 opt.text = "shr11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr_-1_0"
 opt.text = "shr_-1_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr_-1_1"
 opt.text = "shr_-1_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr_-1_255"
 opt.text = "shr_-1_255"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr_-1_256"
 opt.text = "shr_-1_256"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr_2^255_1"
 opt.text = "shr_2^255_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr_2^255_255"
 opt.text = "shr_2^255_255"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr_2^255_256"
 opt.text = "shr_2^255_256"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "shr_2^255_257"
 opt.text = "shr_2^255_257"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stCallCreateCallCodeTest") { 
 var opt = document.createElement("option")
 opt.value = "Call1024BalanceTooLow"
 opt.text = "Call1024BalanceTooLow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call1024OOG"
 opt.text = "Call1024OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call1024PreCalls"
 opt.text = "Call1024PreCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallLoseGasOOG"
 opt.text = "CallLoseGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBombPreCall"
 opt.text = "CallRecursiveBombPreCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Callcode1024BalanceTooLow"
 opt.text = "Callcode1024BalanceTooLow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Callcode1024OOG"
 opt.text = "Callcode1024OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallcodeLoseGasOOG"
 opt.text = "CallcodeLoseGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput1"
 opt.text = "callOutput1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput2"
 opt.text = "callOutput2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput3Fail"
 opt.text = "callOutput3Fail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput3"
 opt.text = "callOutput3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput3partialFail"
 opt.text = "callOutput3partialFail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callOutput3partial"
 opt.text = "callOutput3partial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callWithHighValueAndGasOOG"
 opt.text = "callWithHighValueAndGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callWithHighValueAndOOGatTxLevel"
 opt.text = "callWithHighValueAndOOGatTxLevel"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callWithHighValue"
 opt.text = "callWithHighValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callWithHighValueOOGinCall"
 opt.text = "callWithHighValueOOGinCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeOutput1"
 opt.text = "callcodeOutput1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeOutput2"
 opt.text = "callcodeOutput2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeOutput3Fail"
 opt.text = "callcodeOutput3Fail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeOutput3"
 opt.text = "callcodeOutput3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeOutput3partialFail"
 opt.text = "callcodeOutput3partialFail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeOutput3partial"
 opt.text = "callcodeOutput3partial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeWithHighValueAndGasOOG"
 opt.text = "callcodeWithHighValueAndGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeWithHighValue"
 opt.text = "callcodeWithHighValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "contractCreationMakeCallThatAskMoreGasThenTransactionProvided"
 opt.text = "contractCreationMakeCallThatAskMoreGasThenTransactionProvided"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createFailBalanceTooLow"
 opt.text = "createFailBalanceTooLow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitFailBadJumpDestination2"
 opt.text = "createInitFailBadJumpDestination2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitFailBadJumpDestination"
 opt.text = "createInitFailBadJumpDestination"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitFailStackSizeLargerThan1024"
 opt.text = "createInitFailStackSizeLargerThan1024"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitFailStackUnderflow"
 opt.text = "createInitFailStackUnderflow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitFailUndefinedInstruction2"
 opt.text = "createInitFailUndefinedInstruction2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitFailUndefinedInstruction"
 opt.text = "createInitFailUndefinedInstruction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitFail_OOGduringInit2"
 opt.text = "createInitFail_OOGduringInit2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitFail_OOGduringInit"
 opt.text = "createInitFail_OOGduringInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createInitOOGforCREATE"
 opt.text = "createInitOOGforCREATE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createJS_ExampleContract"
 opt.text = "createJS_ExampleContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createJS_NoCollision"
 opt.text = "createJS_NoCollision"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorPerTxs"
 opt.text = "createNameRegistratorPerTxs"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorPerTxsNotEnoughGas"
 opt.text = "createNameRegistratorPerTxsNotEnoughGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorPreStore1NotEnoughGas"
 opt.text = "createNameRegistratorPreStore1NotEnoughGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorendowmentTooHigh"
 opt.text = "createNameRegistratorendowmentTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stRevertTest") { 
 var opt = document.createElement("option")
 opt.value = "LoopCallsDepthThenRevert2"
 opt.text = "LoopCallsDepthThenRevert2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "LoopCallsDepthThenRevert3"
 opt.text = "LoopCallsDepthThenRevert3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "LoopCallsDepthThenRevert"
 opt.text = "LoopCallsDepthThenRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "LoopCallsThenRevert"
 opt.text = "LoopCallsThenRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "LoopDelegateCallsDepthThenRevert"
 opt.text = "LoopDelegateCallsDepthThenRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NashatyrevSuicideRevert"
 opt.text = "NashatyrevSuicideRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "PythonRevertTestTue201814-1430"
 opt.text = "PythonRevertTestTue201814-1430"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertDepth2"
 opt.text = "RevertDepth2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertDepthCreateAddressCollision"
 opt.text = "RevertDepthCreateAddressCollision"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertDepthCreateOOG"
 opt.text = "RevertDepthCreateOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertInCallCode"
 opt.text = "RevertInCallCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertInCreateInInit"
 opt.text = "RevertInCreateInInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertInDelegateCall"
 opt.text = "RevertInDelegateCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertInStaticCall"
 opt.text = "RevertInStaticCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOnEmptyStack"
 opt.text = "RevertOnEmptyStack"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeCalls"
 opt.text = "RevertOpcodeCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeCreate"
 opt.text = "RevertOpcodeCreate"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeDirectCall"
 opt.text = "RevertOpcodeDirectCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcode"
 opt.text = "RevertOpcode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeInCallsOnNonEmptyReturnData"
 opt.text = "RevertOpcodeInCallsOnNonEmptyReturnData"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeInCreateReturns"
 opt.text = "RevertOpcodeInCreateReturns"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeInInit"
 opt.text = "RevertOpcodeInInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeMultipleSubCalls"
 opt.text = "RevertOpcodeMultipleSubCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeReturn"
 opt.text = "RevertOpcodeReturn"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeWithBigOutputInInit"
 opt.text = "RevertOpcodeWithBigOutputInInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrecompiledTouchExactOOG"
 opt.text = "RevertPrecompiledTouchExactOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrecompiledTouch"
 opt.text = "RevertPrecompiledTouch"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrecompiledTouch_nonce"
 opt.text = "RevertPrecompiledTouch_nonce"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrecompiledTouch_noncestorage"
 opt.text = "RevertPrecompiledTouch_noncestorage"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrecompiledTouch_storage"
 opt.text = "RevertPrecompiledTouch_storage"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrefoundCall"
 opt.text = "RevertPrefoundCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrefoundCallOOG"
 opt.text = "RevertPrefoundCallOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrefoundEmptyCall"
 opt.text = "RevertPrefoundEmptyCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrefoundEmptyCallOOG"
 opt.text = "RevertPrefoundEmptyCallOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrefoundEmpty"
 opt.text = "RevertPrefoundEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrefoundEmptyOOG"
 opt.text = "RevertPrefoundEmptyOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrefound"
 opt.text = "RevertPrefound"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertPrefoundOOG"
 opt.text = "RevertPrefoundOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertRemoteSubCallStorageOOG"
 opt.text = "RevertRemoteSubCallStorageOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertSubCallStorageOOG2"
 opt.text = "RevertSubCallStorageOOG2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertSubCallStorageOOG"
 opt.text = "RevertSubCallStorageOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TouchToEmptyAccountRevert2"
 opt.text = "TouchToEmptyAccountRevert2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TouchToEmptyAccountRevert3"
 opt.text = "TouchToEmptyAccountRevert3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TouchToEmptyAccountRevert"
 opt.text = "TouchToEmptyAccountRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "costRevert"
 opt.text = "costRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stateRevert"
 opt.text = "stateRevert"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stCallCodes") { 
 var opt = document.createElement("option")
 opt.value = "call_OOG_additionalGasCosts1"
 opt.text = "call_OOG_additionalGasCosts1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "call_OOG_additionalGasCosts2"
 opt.text = "call_OOG_additionalGasCosts2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcall_00"
 opt.text = "callcall_00"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcall_00_OOGE"
 opt.text = "callcall_00_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcall_00_OOGE_valueTransfer"
 opt.text = "callcall_00_OOGE_valueTransfer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcall_00_SuicideEnd"
 opt.text = "callcall_00_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcall_000"
 opt.text = "callcallcall_000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcall_000_OOGE"
 opt.text = "callcallcall_000_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcall_000_OOGMAfter"
 opt.text = "callcallcall_000_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcall_000_OOGMBefore"
 opt.text = "callcallcall_000_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcall_000_SuicideEnd"
 opt.text = "callcallcall_000_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcall_000_SuicideMiddle"
 opt.text = "callcallcall_000_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcall_ABCB_RECURSIVE"
 opt.text = "callcallcall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001"
 opt.text = "callcallcallcode_001"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGE"
 opt.text = "callcallcallcode_001_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGMAfter"
 opt.text = "callcallcallcode_001_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGMBefore"
 opt.text = "callcallcallcode_001_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_SuicideEnd"
 opt.text = "callcallcallcode_001_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_SuicideMiddle"
 opt.text = "callcallcallcode_001_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_ABCB_RECURSIVE"
 opt.text = "callcallcallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01"
 opt.text = "callcallcode_01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01_OOGE"
 opt.text = "callcallcode_01_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01_SuicideEnd"
 opt.text = "callcallcode_01_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010"
 opt.text = "callcallcodecall_010"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGE"
 opt.text = "callcallcodecall_010_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGMAfter"
 opt.text = "callcallcodecall_010_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGMBefore"
 opt.text = "callcallcodecall_010_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_SuicideEnd"
 opt.text = "callcallcodecall_010_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_SuicideMiddle"
 opt.text = "callcallcodecall_010_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_ABCB_RECURSIVE"
 opt.text = "callcallcodecall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011"
 opt.text = "callcallcodecallcode_011"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGE"
 opt.text = "callcallcodecallcode_011_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGMAfter"
 opt.text = "callcallcodecallcode_011_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGMBefore"
 opt.text = "callcallcodecallcode_011_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_SuicideEnd"
 opt.text = "callcallcodecallcode_011_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_SuicideMiddle"
 opt.text = "callcallcodecallcode_011_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_ABCB_RECURSIVE"
 opt.text = "callcallcodecallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeDynamicCode2SelfCall"
 opt.text = "callcodeDynamicCode2SelfCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeDynamicCode"
 opt.text = "callcodeDynamicCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeEmptycontract"
 opt.text = "callcodeEmptycontract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeInInitcodeToEmptyContract"
 opt.text = "callcodeInInitcodeToEmptyContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeInInitcodeToExisContractWithVTransferNEMoney"
 opt.text = "callcodeInInitcodeToExisContractWithVTransferNEMoney"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeInInitcodeToExistingContract"
 opt.text = "callcodeInInitcodeToExistingContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeInInitcodeToExistingContractWithValueTransfer"
 opt.text = "callcodeInInitcodeToExistingContractWithValueTransfer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcode_checkPC"
 opt.text = "callcode_checkPC"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10"
 opt.text = "callcodecall_10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10_OOGE"
 opt.text = "callcodecall_10_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10_SuicideEnd"
 opt.text = "callcodecall_10_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100"
 opt.text = "callcodecallcall_100"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGE"
 opt.text = "callcodecallcall_100_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGMAfter"
 opt.text = "callcodecallcall_100_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGMBefore"
 opt.text = "callcodecallcall_100_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_SuicideEnd"
 opt.text = "callcodecallcall_100_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_SuicideMiddle"
 opt.text = "callcodecallcall_100_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_ABCB_RECURSIVE"
 opt.text = "callcodecallcall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101"
 opt.text = "callcodecallcallcode_101"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGE"
 opt.text = "callcodecallcallcode_101_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGMAfter"
 opt.text = "callcodecallcallcode_101_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGMBefore"
 opt.text = "callcodecallcallcode_101_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_SuicideEnd"
 opt.text = "callcodecallcallcode_101_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_SuicideMiddle"
 opt.text = "callcodecallcallcode_101_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_ABCB_RECURSIVE"
 opt.text = "callcodecallcallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11"
 opt.text = "callcodecallcode_11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11_OOGE"
 opt.text = "callcodecallcode_11_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11_SuicideEnd"
 opt.text = "callcodecallcode_11_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110"
 opt.text = "callcodecallcodecall_110"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGE"
 opt.text = "callcodecallcodecall_110_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGMAfter"
 opt.text = "callcodecallcodecall_110_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGMBefore"
 opt.text = "callcodecallcodecall_110_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_SuicideEnd"
 opt.text = "callcodecallcodecall_110_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_SuicideMiddle"
 opt.text = "callcodecallcodecall_110_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_ABCB_RECURSIVE"
 opt.text = "callcodecallcodecall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111"
 opt.text = "callcodecallcodecallcode_111"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGE"
 opt.text = "callcodecallcodecallcode_111_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGMAfter"
 opt.text = "callcodecallcodecallcode_111_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGMBefore"
 opt.text = "callcodecallcodecallcode_111_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_SuicideEnd"
 opt.text = "callcodecallcodecallcode_111_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_SuicideMiddle"
 opt.text = "callcodecallcodecallcode_111_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_ABCB_RECURSIVE"
 opt.text = "callcodecallcodecallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "touchAndGo"
 opt.text = "touchAndGo"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stEIP158Specific") { 
 var opt = document.createElement("option")
 opt.value = "CALL_OneVCallSuicide"
 opt.text = "CALL_OneVCallSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CALL_ZeroVCallSuicide"
 opt.text = "CALL_ZeroVCallSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "EXP_Empty"
 opt.text = "EXP_Empty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "EXTCODESIZE_toEpmty"
 opt.text = "EXTCODESIZE_toEpmty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "EXTCODESIZE_toNonExistent"
 opt.text = "EXTCODESIZE_toNonExistent"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callToEmptyThenCallError"
 opt.text = "callToEmptyThenCallError"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "vitalikTransactionTest"
 opt.text = "vitalikTransactionTest"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stCallDelegateCodesCallCodeHomestead") { 
 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001"
 opt.text = "callcallcallcode_001"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGE"
 opt.text = "callcallcallcode_001_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGMAfter"
 opt.text = "callcallcallcode_001_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_OOGMBefore"
 opt.text = "callcallcallcode_001_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_SuicideEnd"
 opt.text = "callcallcallcode_001_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_001_SuicideMiddle"
 opt.text = "callcallcallcode_001_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcallcode_ABCB_RECURSIVE"
 opt.text = "callcallcallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01"
 opt.text = "callcallcode_01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01_OOGE"
 opt.text = "callcallcode_01_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcode_01_SuicideEnd"
 opt.text = "callcallcode_01_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010"
 opt.text = "callcallcodecall_010"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGE"
 opt.text = "callcallcodecall_010_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGMAfter"
 opt.text = "callcallcodecall_010_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_OOGMBefore"
 opt.text = "callcallcodecall_010_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_SuicideEnd"
 opt.text = "callcallcodecall_010_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_010_SuicideMiddle"
 opt.text = "callcallcodecall_010_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecall_ABCB_RECURSIVE"
 opt.text = "callcallcodecall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011"
 opt.text = "callcallcodecallcode_011"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGE"
 opt.text = "callcallcodecallcode_011_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGMAfter"
 opt.text = "callcallcodecallcode_011_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_OOGMBefore"
 opt.text = "callcallcodecallcode_011_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_SuicideEnd"
 opt.text = "callcallcodecallcode_011_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_011_SuicideMiddle"
 opt.text = "callcallcodecallcode_011_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcallcodecallcode_ABCB_RECURSIVE"
 opt.text = "callcallcodecallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10"
 opt.text = "callcodecall_10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10_OOGE"
 opt.text = "callcodecall_10_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecall_10_SuicideEnd"
 opt.text = "callcodecall_10_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100"
 opt.text = "callcodecallcall_100"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGE"
 opt.text = "callcodecallcall_100_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGMAfter"
 opt.text = "callcodecallcall_100_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_OOGMBefore"
 opt.text = "callcodecallcall_100_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_SuicideEnd"
 opt.text = "callcodecallcall_100_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_100_SuicideMiddle"
 opt.text = "callcodecallcall_100_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcall_ABCB_RECURSIVE"
 opt.text = "callcodecallcall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101"
 opt.text = "callcodecallcallcode_101"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGE"
 opt.text = "callcodecallcallcode_101_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGMAfter"
 opt.text = "callcodecallcallcode_101_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_OOGMBefore"
 opt.text = "callcodecallcallcode_101_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_SuicideEnd"
 opt.text = "callcodecallcallcode_101_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_101_SuicideMiddle"
 opt.text = "callcodecallcallcode_101_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcallcode_ABCB_RECURSIVE"
 opt.text = "callcodecallcallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11"
 opt.text = "callcodecallcode_11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11_OOGE"
 opt.text = "callcodecallcode_11_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcode_11_SuicideEnd"
 opt.text = "callcodecallcode_11_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110"
 opt.text = "callcodecallcodecall_110"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGE"
 opt.text = "callcodecallcodecall_110_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGMAfter"
 opt.text = "callcodecallcodecall_110_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_OOGMBefore"
 opt.text = "callcodecallcodecall_110_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_SuicideEnd"
 opt.text = "callcodecallcodecall_110_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_110_SuicideMiddle"
 opt.text = "callcodecallcodecall_110_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecall_ABCB_RECURSIVE"
 opt.text = "callcodecallcodecall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111"
 opt.text = "callcodecallcodecallcode_111"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGE"
 opt.text = "callcodecallcodecallcode_111_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGMAfter"
 opt.text = "callcodecallcodecallcode_111_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_OOGMBefore"
 opt.text = "callcodecallcodecallcode_111_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_SuicideEnd"
 opt.text = "callcodecallcodecallcode_111_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_111_SuicideMiddle"
 opt.text = "callcodecallcodecallcode_111_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodecallcodecallcode_ABCB_RECURSIVE"
 opt.text = "callcodecallcodecallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stTransactionTest") { 
 var opt = document.createElement("option")
 opt.value = "ContractStoreClearsOOG"
 opt.text = "ContractStoreClearsOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ContractStoreClearsSuccess"
 opt.text = "ContractStoreClearsSuccess"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateMessageReverted"
 opt.text = "CreateMessageReverted"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateMessageSuccess"
 opt.text = "CreateMessageSuccess"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateTransactionEOF1"
 opt.text = "CreateTransactionEOF1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateTransactionSuccess"
 opt.text = "CreateTransactionSuccess"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "EmptyTransaction3"
 opt.text = "EmptyTransaction3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "HighGasLimit"
 opt.text = "HighGasLimit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "HighGasPrice"
 opt.text = "HighGasPrice"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "InternalCallHittingGasLimit2"
 opt.text = "InternalCallHittingGasLimit2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "InternalCallHittingGasLimit"
 opt.text = "InternalCallHittingGasLimit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "InternalCallHittingGasLimitSuccess"
 opt.text = "InternalCallHittingGasLimitSuccess"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "InternlCallStoreClearsOOG"
 opt.text = "InternlCallStoreClearsOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "InternlCallStoreClearsSucces"
 opt.text = "InternlCallStoreClearsSucces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Opcodes_TransactionInit"
 opt.text = "Opcodes_TransactionInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "OverflowGasRequire2"
 opt.text = "OverflowGasRequire2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "PointAtInfinityECRecover"
 opt.text = "PointAtInfinityECRecover"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "StoreClearsAndInternlCallStoreClearsOOG"
 opt.text = "StoreClearsAndInternlCallStoreClearsOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "StoreClearsAndInternlCallStoreClearsSuccess"
 opt.text = "StoreClearsAndInternlCallStoreClearsSuccess"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "StoreGasOnCreate"
 opt.text = "StoreGasOnCreate"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SuicidesAndInternlCallSuicidesBonusGasAtCallFailed"
 opt.text = "SuicidesAndInternlCallSuicidesBonusGasAtCallFailed"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SuicidesAndInternlCallSuicidesBonusGasAtCall"
 opt.text = "SuicidesAndInternlCallSuicidesBonusGasAtCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SuicidesAndInternlCallSuicidesOOG"
 opt.text = "SuicidesAndInternlCallSuicidesOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SuicidesAndInternlCallSuicidesSuccess"
 opt.text = "SuicidesAndInternlCallSuicidesSuccess"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SuicidesAndSendMoneyToItselfEtherDestroyed"
 opt.text = "SuicidesAndSendMoneyToItselfEtherDestroyed"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SuicidesStopAfterSuicide"
 opt.text = "SuicidesStopAfterSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionDataCosts652"
 opt.text = "TransactionDataCosts652"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionSendingToEmpty"
 opt.text = "TransactionSendingToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionSendingToZero"
 opt.text = "TransactionSendingToZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionToAddressh160minusOne"
 opt.text = "TransactionToAddressh160minusOne"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionToItself"
 opt.text = "TransactionToItself"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ValueOverflow"
 opt.text = "ValueOverflow"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stEIP150singleCodeGasPrices") { 
 var opt = document.createElement("option")
 opt.value = "RawBalanceGas"
 opt.text = "RawBalanceGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallCodeGasAsk"
 opt.text = "RawCallCodeGasAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallCodeGas"
 opt.text = "RawCallCodeGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallCodeGasMemoryAsk"
 opt.text = "RawCallCodeGasMemoryAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallCodeGasMemory"
 opt.text = "RawCallCodeGasMemory"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallCodeGasValueTransferAsk"
 opt.text = "RawCallCodeGasValueTransferAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallCodeGasValueTransfer"
 opt.text = "RawCallCodeGasValueTransfer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallCodeGasValueTransferMemoryAsk"
 opt.text = "RawCallCodeGasValueTransferMemoryAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallCodeGasValueTransferMemory"
 opt.text = "RawCallCodeGasValueTransferMemory"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallGasAsk"
 opt.text = "RawCallGasAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallGas"
 opt.text = "RawCallGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallGasValueTransferAsk"
 opt.text = "RawCallGasValueTransferAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallGasValueTransfer"
 opt.text = "RawCallGasValueTransfer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallGasValueTransferMemoryAsk"
 opt.text = "RawCallGasValueTransferMemoryAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallGasValueTransferMemory"
 opt.text = "RawCallGasValueTransferMemory"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallMemoryGasAsk"
 opt.text = "RawCallMemoryGasAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCallMemoryGas"
 opt.text = "RawCallMemoryGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCreateFailGasValueTransfer2"
 opt.text = "RawCreateFailGasValueTransfer2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCreateFailGasValueTransfer"
 opt.text = "RawCreateFailGasValueTransfer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCreateGas"
 opt.text = "RawCreateGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCreateGasMemory"
 opt.text = "RawCreateGasMemory"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCreateGasValueTransfer"
 opt.text = "RawCreateGasValueTransfer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawCreateGasValueTransferMemory"
 opt.text = "RawCreateGasValueTransferMemory"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawDelegateCallGasAsk"
 opt.text = "RawDelegateCallGasAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawDelegateCallGas"
 opt.text = "RawDelegateCallGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawDelegateCallGasMemoryAsk"
 opt.text = "RawDelegateCallGasMemoryAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawDelegateCallGasMemory"
 opt.text = "RawDelegateCallGasMemory"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawExtCodeCopyGas"
 opt.text = "RawExtCodeCopyGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawExtCodeCopyMemoryGas"
 opt.text = "RawExtCodeCopyMemoryGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RawExtCodeSizeGas"
 opt.text = "RawExtCodeSizeGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "eip2929-ff"
 opt.text = "eip2929-ff"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "eip2929"
 opt.text = "eip2929"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "eip2929OOG"
 opt.text = "eip2929OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasCostBerlin"
 opt.text = "gasCostBerlin"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasCostExp"
 opt.text = "gasCostExp"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasCost"
 opt.text = "gasCost"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasCostJump"
 opt.text = "gasCostJump"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasCostMemSeg"
 opt.text = "gasCostMemSeg"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasCostMemory"
 opt.text = "gasCostMemory"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasCostReturn"
 opt.text = "gasCostReturn"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stSLoadTest") { 
 var opt = document.createElement("option")
 opt.value = "sloadGasCost"
 opt.text = "sloadGasCost"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stMemoryTest") { 
 var opt = document.createElement("option")
 opt.value = "buffer"
 opt.text = "buffer"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "bufferSrcOffset"
 opt.text = "bufferSrcOffset"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callDataCopyOffset"
 opt.text = "callDataCopyOffset"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "calldatacopy_dejavu2"
 opt.text = "calldatacopy_dejavu2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "calldatacopy_dejavu"
 opt.text = "calldatacopy_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "codeCopyOffset"
 opt.text = "codeCopyOffset"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "codecopy_dejavu2"
 opt.text = "codecopy_dejavu2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "codecopy_dejavu"
 opt.text = "codecopy_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extcodecopy_dejavu"
 opt.text = "extcodecopy_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log1_dejavu"
 opt.text = "log1_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log2_dejavu"
 opt.text = "log2_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log3_dejavu"
 opt.text = "log3_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "log4_dejavu"
 opt.text = "log4_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem0b_singleByte"
 opt.text = "mem0b_singleByte"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem31b_singleByte"
 opt.text = "mem31b_singleByte"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32b_singleByte"
 opt.text = "mem32b_singleByte"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb+1"
 opt.text = "mem32kb+1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb+31"
 opt.text = "mem32kb+31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb+32"
 opt.text = "mem32kb+32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb+33"
 opt.text = "mem32kb+33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb-1"
 opt.text = "mem32kb-1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb-31"
 opt.text = "mem32kb-31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb-32"
 opt.text = "mem32kb-32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb-33"
 opt.text = "mem32kb-33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb"
 opt.text = "mem32kb"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte+1"
 opt.text = "mem32kb_singleByte+1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte+31"
 opt.text = "mem32kb_singleByte+31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte+32"
 opt.text = "mem32kb_singleByte+32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte+33"
 opt.text = "mem32kb_singleByte+33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte-1"
 opt.text = "mem32kb_singleByte-1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte-31"
 opt.text = "mem32kb_singleByte-31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte-32"
 opt.text = "mem32kb_singleByte-32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte-33"
 opt.text = "mem32kb_singleByte-33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem32kb_singleByte"
 opt.text = "mem32kb_singleByte"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem33b_singleByte"
 opt.text = "mem33b_singleByte"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb+1"
 opt.text = "mem64kb+1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb+31"
 opt.text = "mem64kb+31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb+32"
 opt.text = "mem64kb+32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb+33"
 opt.text = "mem64kb+33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb-1"
 opt.text = "mem64kb-1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb-31"
 opt.text = "mem64kb-31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb-32"
 opt.text = "mem64kb-32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb-33"
 opt.text = "mem64kb-33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb"
 opt.text = "mem64kb"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte+1"
 opt.text = "mem64kb_singleByte+1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte+31"
 opt.text = "mem64kb_singleByte+31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte+32"
 opt.text = "mem64kb_singleByte+32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte+33"
 opt.text = "mem64kb_singleByte+33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte-1"
 opt.text = "mem64kb_singleByte-1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte-31"
 opt.text = "mem64kb_singleByte-31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte-32"
 opt.text = "mem64kb_singleByte-32"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte-33"
 opt.text = "mem64kb_singleByte-33"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mem64kb_singleByte"
 opt.text = "mem64kb_singleByte"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "memCopySelf"
 opt.text = "memCopySelf"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "memReturn"
 opt.text = "memReturn"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mload16bitBound"
 opt.text = "mload16bitBound"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mload8bitBound"
 opt.text = "mload8bitBound"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mload_dejavu"
 opt.text = "mload_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mstore_dejavu"
 opt.text = "mstore_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "mstroe8_dejavu"
 opt.text = "mstroe8_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "oog"
 opt.text = "oog"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sha3_dejavu"
 opt.text = "sha3_dejavu"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitGas_1023"
 opt.text = "stackLimitGas_1023"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitGas_1024"
 opt.text = "stackLimitGas_1024"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitGas_1025"
 opt.text = "stackLimitGas_1025"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitPush31_1023"
 opt.text = "stackLimitPush31_1023"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitPush31_1024"
 opt.text = "stackLimitPush31_1024"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitPush31_1025"
 opt.text = "stackLimitPush31_1025"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitPush32_1023"
 opt.text = "stackLimitPush32_1023"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitPush32_1024"
 opt.text = "stackLimitPush32_1024"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stackLimitPush32_1025"
 opt.text = "stackLimitPush32_1025"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stEIP3607") { 
 var opt = document.createElement("option")
 opt.value = "initCollidingWithNonEmptyAccount"
 opt.text = "initCollidingWithNonEmptyAccount"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "transactionCollidingWithNonEmptyAccount_calls"
 opt.text = "transactionCollidingWithNonEmptyAccount_calls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "transactionCollidingWithNonEmptyAccount_callsItself"
 opt.text = "transactionCollidingWithNonEmptyAccount_callsItself"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "transactionCollidingWithNonEmptyAccount_init"
 opt.text = "transactionCollidingWithNonEmptyAccount_init"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "transactionCollidingWithNonEmptyAccount_send"
 opt.text = "transactionCollidingWithNonEmptyAccount_send"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "VMTests") { 
 var opt = document.createElement("option")
 opt.value = "dummy"
 opt.text = "dummy"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stRefundTest") { 
 var opt = document.createElement("option")
 opt.value = "refund50_1"
 opt.text = "refund50_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund50_2"
 opt.text = "refund50_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund50percentCap"
 opt.text = "refund50percentCap"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund600"
 opt.text = "refund600"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refundFF"
 opt.text = "refundFF"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refundMax"
 opt.text = "refundMax"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refundSSTORE"
 opt.text = "refundSSTORE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refundSuicide50procentCap"
 opt.text = "refundSuicide50procentCap"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_CallA"
 opt.text = "refund_CallA"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_CallA_OOG"
 opt.text = "refund_CallA_OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_CallA_notEnoughGasInCall"
 opt.text = "refund_CallA_notEnoughGasInCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_CallToSuicideNoStorage"
 opt.text = "refund_CallToSuicideNoStorage"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_CallToSuicideStorage"
 opt.text = "refund_CallToSuicideStorage"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_CallToSuicideTwice"
 opt.text = "refund_CallToSuicideTwice"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_NoOOG_1"
 opt.text = "refund_NoOOG_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_OOG"
 opt.text = "refund_OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_TxToSuicide"
 opt.text = "refund_TxToSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_TxToSuicideOOG"
 opt.text = "refund_TxToSuicideOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_changeNonZeroStorage"
 opt.text = "refund_changeNonZeroStorage"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_getEtherBack"
 opt.text = "refund_getEtherBack"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_multimpleSuicide"
 opt.text = "refund_multimpleSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "refund_singleSuicide"
 opt.text = "refund_singleSuicide"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stInitCodeTest") { 
 var opt = document.createElement("option")
 opt.value = "CallContractToCreateContractAndCallItOOG"
 opt.text = "CallContractToCreateContractAndCallItOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallContractToCreateContractNoCash"
 opt.text = "CallContractToCreateContractNoCash"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallContractToCreateContractOOGBonusGas"
 opt.text = "CallContractToCreateContractOOGBonusGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallContractToCreateContractOOG"
 opt.text = "CallContractToCreateContractOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallContractToCreateContractWhichWouldCreateContractIfCalled"
 opt.text = "CallContractToCreateContractWhichWouldCreateContractIfCalled"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallContractToCreateContractWhichWouldCreateContractInInitCode"
 opt.text = "CallContractToCreateContractWhichWouldCreateContractInInitCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveContract"
 opt.text = "CallRecursiveContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallTheContractToCreateEmptyContract"
 opt.text = "CallTheContractToCreateEmptyContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "OutOfGasContractCreation"
 opt.text = "OutOfGasContractCreation"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "OutOfGasPrefundedContractCreation"
 opt.text = "OutOfGasPrefundedContractCreation"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ReturnTest2"
 opt.text = "ReturnTest2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ReturnTest"
 opt.text = "ReturnTest"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "StackUnderFlowContractCreation"
 opt.text = "StackUnderFlowContractCreation"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionCreateAutoSuicideContract"
 opt.text = "TransactionCreateAutoSuicideContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionCreateRandomInitCode"
 opt.text = "TransactionCreateRandomInitCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionCreateStopInInitcode"
 opt.text = "TransactionCreateStopInInitcode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TransactionCreateSuicideInInitcode"
 opt.text = "TransactionCreateSuicideInInitcode"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stNonZeroCallsTest") { 
 var opt = document.createElement("option")
 opt.value = "NonZeroValue_CALLCODE"
 opt.text = "NonZeroValue_CALLCODE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_CALLCODE_ToEmpty"
 opt.text = "NonZeroValue_CALLCODE_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_CALLCODE_ToNonNonZeroBalance"
 opt.text = "NonZeroValue_CALLCODE_ToNonNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_CALLCODE_ToOneStorageKey"
 opt.text = "NonZeroValue_CALLCODE_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_CALL"
 opt.text = "NonZeroValue_CALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_CALL_ToEmpty"
 opt.text = "NonZeroValue_CALL_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_CALL_ToNonNonZeroBalance"
 opt.text = "NonZeroValue_CALL_ToNonNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_CALL_ToOneStorageKey"
 opt.text = "NonZeroValue_CALL_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_DELEGATECALL"
 opt.text = "NonZeroValue_DELEGATECALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_DELEGATECALL_ToEmpty"
 opt.text = "NonZeroValue_DELEGATECALL_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_DELEGATECALL_ToNonNonZeroBalance"
 opt.text = "NonZeroValue_DELEGATECALL_ToNonNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_DELEGATECALL_ToOneStorageKey"
 opt.text = "NonZeroValue_DELEGATECALL_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_SUICIDE"
 opt.text = "NonZeroValue_SUICIDE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_SUICIDE_ToEmpty"
 opt.text = "NonZeroValue_SUICIDE_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_SUICIDE_ToNonNonZeroBalance"
 opt.text = "NonZeroValue_SUICIDE_ToNonNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_SUICIDE_ToOneStorageKey"
 opt.text = "NonZeroValue_SUICIDE_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_TransactionCALL"
 opt.text = "NonZeroValue_TransactionCALL"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_TransactionCALL_ToEmpty"
 opt.text = "NonZeroValue_TransactionCALL_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_TransactionCALL_ToNonNonZeroBalance"
 opt.text = "NonZeroValue_TransactionCALL_ToNonNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_TransactionCALL_ToOneStorageKey"
 opt.text = "NonZeroValue_TransactionCALL_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_TransactionCALLwithData"
 opt.text = "NonZeroValue_TransactionCALLwithData"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_TransactionCALLwithData_ToEmpty"
 opt.text = "NonZeroValue_TransactionCALLwithData_ToEmpty"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_TransactionCALLwithData_ToNonNonZeroBalance"
 opt.text = "NonZeroValue_TransactionCALLwithData_ToNonNonZeroBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NonZeroValue_TransactionCALLwithData_ToOneStorageKey"
 opt.text = "NonZeroValue_TransactionCALLwithData_ToOneStorageKey"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stSpecialTest") { 
 var opt = document.createElement("option")
 opt.value = "FailedCreateRevertsDeletion"
 opt.text = "FailedCreateRevertsDeletion"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "JUMPDEST_Attack"
 opt.text = "JUMPDEST_Attack"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "JUMPDEST_AttackwithJump"
 opt.text = "JUMPDEST_AttackwithJump"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "OverflowGasMakeMoney"
 opt.text = "OverflowGasMakeMoney"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "StackDepthLimitSEC"
 opt.text = "StackDepthLimitSEC"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "block504980"
 opt.text = "block504980"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "deploymentError"
 opt.text = "deploymentError"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "failed_tx_xcf416c53"
 opt.text = "failed_tx_xcf416c53"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "gasPrice0"
 opt.text = "gasPrice0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "makeMoney"
 opt.text = "makeMoney"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "push32withoutByte"
 opt.text = "push32withoutByte"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "selfdestructEIP2929"
 opt.text = "selfdestructEIP2929"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sha3_deja"
 opt.text = "sha3_deja"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "tx_e1c174e2"
 opt.text = "tx_e1c174e2"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stStaticFlagEnabled") { 
 var opt = document.createElement("option")
 opt.value = "CallWithNOTZeroValueToPrecompileFromCalledContract"
 opt.text = "CallWithNOTZeroValueToPrecompileFromCalledContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallWithNOTZeroValueToPrecompileFromContractInitialization"
 opt.text = "CallWithNOTZeroValueToPrecompileFromContractInitialization"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallWithNOTZeroValueToPrecompileFromTransaction"
 opt.text = "CallWithNOTZeroValueToPrecompileFromTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallWithZeroValueToPrecompileFromCalledContract"
 opt.text = "CallWithZeroValueToPrecompileFromCalledContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallWithZeroValueToPrecompileFromContractInitialization"
 opt.text = "CallWithZeroValueToPrecompileFromContractInitialization"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallWithZeroValueToPrecompileFromTransaction"
 opt.text = "CallWithZeroValueToPrecompileFromTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallcodeToPrecompileFromCalledContract"
 opt.text = "CallcodeToPrecompileFromCalledContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallcodeToPrecompileFromContractInitialization"
 opt.text = "CallcodeToPrecompileFromContractInitialization"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallcodeToPrecompileFromTransaction"
 opt.text = "CallcodeToPrecompileFromTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DelegatecallToPrecompileFromCalledContract"
 opt.text = "DelegatecallToPrecompileFromCalledContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DelegatecallToPrecompileFromContractInitialization"
 opt.text = "DelegatecallToPrecompileFromContractInitialization"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DelegatecallToPrecompileFromTransaction"
 opt.text = "DelegatecallToPrecompileFromTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "StaticcallForPrecompilesIssue683"
 opt.text = "StaticcallForPrecompilesIssue683"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stEIP150Specific") { 
 var opt = document.createElement("option")
 opt.value = "CallAndCallcodeConsumeMoreGasThenTransactionHas"
 opt.text = "CallAndCallcodeConsumeMoreGasThenTransactionHas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallAskMoreGasOnDepth2ThenTransactionHas"
 opt.text = "CallAskMoreGasOnDepth2ThenTransactionHas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallGoesOOGOnSecondLevel2"
 opt.text = "CallGoesOOGOnSecondLevel2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallGoesOOGOnSecondLevel"
 opt.text = "CallGoesOOGOnSecondLevel"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateAndGasInsideCreate"
 opt.text = "CreateAndGasInsideCreate"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "DelegateCallOnEIP"
 opt.text = "DelegateCallOnEIP"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ExecuteCallThatAskForeGasThenTrabsactionHas"
 opt.text = "ExecuteCallThatAskForeGasThenTrabsactionHas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "NewGasPriceForCodes"
 opt.text = "NewGasPriceForCodes"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SuicideToExistingContract"
 opt.text = "SuicideToExistingContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "SuicideToNotExistingContract"
 opt.text = "SuicideToNotExistingContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Transaction64Rule_d64e0"
 opt.text = "Transaction64Rule_d64e0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Transaction64Rule_d64m1"
 opt.text = "Transaction64Rule_d64m1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Transaction64Rule_d64p1"
 opt.text = "Transaction64Rule_d64p1"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stExpectSection") { 
 var opt = document.createElement("option")
 opt.value = "result_eip1559EnvConvertion"
 opt.text = "result_eip1559EnvConvertion"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "result_legacyEnvConvertion"
 opt.text = "result_legacyEnvConvertion"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "result_mergeEnvConvertion"
 opt.text = "result_mergeEnvConvertion"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stEIP2930") { 
 var opt = document.createElement("option")
 opt.value = "addressOpcodes"
 opt.text = "addressOpcodes"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "coinbaseT01"
 opt.text = "coinbaseT01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "coinbaseT2"
 opt.text = "coinbaseT2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "manualCreate"
 opt.text = "manualCreate"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "storageCosts"
 opt.text = "storageCosts"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "transactionCosts"
 opt.text = "transactionCosts"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "variedContext"
 opt.text = "variedContext"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stSystemOperationsTest") { 
 var opt = document.createElement("option")
 opt.value = "ABAcalls0"
 opt.text = "ABAcalls0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ABAcalls1"
 opt.text = "ABAcalls1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ABAcalls2"
 opt.text = "ABAcalls2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ABAcalls3"
 opt.text = "ABAcalls3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ABAcallsSuicide0"
 opt.text = "ABAcallsSuicide0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ABAcallsSuicide1"
 opt.text = "ABAcallsSuicide1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Call10"
 opt.text = "Call10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBomb0"
 opt.text = "CallRecursiveBomb0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBomb0_OOG_atMaxCallDepth"
 opt.text = "CallRecursiveBomb0_OOG_atMaxCallDepth"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBomb1"
 opt.text = "CallRecursiveBomb1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBomb2"
 opt.text = "CallRecursiveBomb2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBomb3"
 opt.text = "CallRecursiveBomb3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBombLog2"
 opt.text = "CallRecursiveBombLog2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallRecursiveBombLog"
 opt.text = "CallRecursiveBombLog"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistrator0"
 opt.text = "CallToNameRegistrator0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorAddressTooBigLeft"
 opt.text = "CallToNameRegistratorAddressTooBigLeft"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorAddressTooBigRight"
 opt.text = "CallToNameRegistratorAddressTooBigRight"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorMemOOGAndInsufficientBalance"
 opt.text = "CallToNameRegistratorMemOOGAndInsufficientBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorNotMuchMemory0"
 opt.text = "CallToNameRegistratorNotMuchMemory0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorNotMuchMemory1"
 opt.text = "CallToNameRegistratorNotMuchMemory1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorOutOfGas"
 opt.text = "CallToNameRegistratorOutOfGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorTooMuchMemory0"
 opt.text = "CallToNameRegistratorTooMuchMemory0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorTooMuchMemory1"
 opt.text = "CallToNameRegistratorTooMuchMemory1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorTooMuchMemory2"
 opt.text = "CallToNameRegistratorTooMuchMemory2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToNameRegistratorZeorSizeMemExpansion"
 opt.text = "CallToNameRegistratorZeorSizeMemExpansion"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToReturn1"
 opt.text = "CallToReturn1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToReturn1ForDynamicJump0"
 opt.text = "CallToReturn1ForDynamicJump0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CallToReturn1ForDynamicJump1"
 opt.text = "CallToReturn1ForDynamicJump1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CalltoReturn2"
 opt.text = "CalltoReturn2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateHashCollision"
 opt.text = "CreateHashCollision"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "PostToReturn1"
 opt.text = "PostToReturn1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "TestNameRegistrator"
 opt.text = "TestNameRegistrator"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "balanceInputAddressTooBig"
 opt.text = "balanceInputAddressTooBig"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callValue"
 opt.text = "callValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeTo0"
 opt.text = "callcodeTo0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeToNameRegistrator0"
 opt.text = "callcodeToNameRegistrator0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeToNameRegistratorAddresTooBigLeft"
 opt.text = "callcodeToNameRegistratorAddresTooBigLeft"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeToNameRegistratorAddresTooBigRight"
 opt.text = "callcodeToNameRegistratorAddresTooBigRight"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeToNameRegistratorZeroMemExpanion"
 opt.text = "callcodeToNameRegistratorZeroMemExpanion"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callcodeToReturn1"
 opt.text = "callcodeToReturn1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "callerAccountBalance"
 opt.text = "callerAccountBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistrator"
 opt.text = "createNameRegistrator"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorOOG_MemExpansionOOV"
 opt.text = "createNameRegistratorOOG_MemExpansionOOV"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorOutOfMemoryBonds0"
 opt.text = "createNameRegistratorOutOfMemoryBonds0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorOutOfMemoryBonds1"
 opt.text = "createNameRegistratorOutOfMemoryBonds1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorValueTooHigh"
 opt.text = "createNameRegistratorValueTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorZeroMem2"
 opt.text = "createNameRegistratorZeroMem2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorZeroMemExpansion"
 opt.text = "createNameRegistratorZeroMemExpansion"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createNameRegistratorZeroMem"
 opt.text = "createNameRegistratorZeroMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "createWithInvalidOpcode"
 opt.text = "createWithInvalidOpcode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "currentAccountBalance"
 opt.text = "currentAccountBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "doubleSelfdestructTest2"
 opt.text = "doubleSelfdestructTest2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "doubleSelfdestructTest"
 opt.text = "doubleSelfdestructTest"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "doubleSelfdestructTouch"
 opt.text = "doubleSelfdestructTouch"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "extcodecopy"
 opt.text = "extcodecopy"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "return0"
 opt.text = "return0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "return1"
 opt.text = "return1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "return2"
 opt.text = "return2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideAddress"
 opt.text = "suicideAddress"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideCallerAddresTooBigLeft"
 opt.text = "suicideCallerAddresTooBigLeft"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideCallerAddresTooBigRight"
 opt.text = "suicideCallerAddresTooBigRight"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideCaller"
 opt.text = "suicideCaller"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideNotExistingAccount"
 opt.text = "suicideNotExistingAccount"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideOrigin"
 opt.text = "suicideOrigin"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideSendEtherPostDeath"
 opt.text = "suicideSendEtherPostDeath"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "suicideSendEtherToMe"
 opt.text = "suicideSendEtherToMe"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "testRandomTest"
 opt.text = "testRandomTest"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stBadOpcode") { 
 var opt = document.createElement("option")
 opt.value = "badOpcodes"
 opt.text = "badOpcodes"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "eip2315NotRemoved"
 opt.text = "eip2315NotRemoved"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "invalidAddr"
 opt.text = "invalidAddr"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "invalidDiffPlaces"
 opt.text = "invalidDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "measureGas"
 opt.text = "measureGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc0CDiffPlaces"
 opt.text = "opc0CDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc0DDiffPlaces"
 opt.text = "opc0DDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc0EDiffPlaces"
 opt.text = "opc0EDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc0FDiffPlaces"
 opt.text = "opc0FDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc1EDiffPlaces"
 opt.text = "opc1EDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc1FDiffPlaces"
 opt.text = "opc1FDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc21DiffPlaces"
 opt.text = "opc21DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc22DiffPlaces"
 opt.text = "opc22DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc23DiffPlaces"
 opt.text = "opc23DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc24DiffPlaces"
 opt.text = "opc24DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc25DiffPlaces"
 opt.text = "opc25DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc26DiffPlaces"
 opt.text = "opc26DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc27DiffPlaces"
 opt.text = "opc27DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc28DiffPlaces"
 opt.text = "opc28DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc29DiffPlaces"
 opt.text = "opc29DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc2ADiffPlaces"
 opt.text = "opc2ADiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc2BDiffPlaces"
 opt.text = "opc2BDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc2CDiffPlaces"
 opt.text = "opc2CDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc2DDiffPlaces"
 opt.text = "opc2DDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc2EDiffPlaces"
 opt.text = "opc2EDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc2FDiffPlaces"
 opt.text = "opc2FDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc49DiffPlaces"
 opt.text = "opc49DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc4ADiffPlaces"
 opt.text = "opc4ADiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc4BDiffPlaces"
 opt.text = "opc4BDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc4CDiffPlaces"
 opt.text = "opc4CDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc4DDiffPlaces"
 opt.text = "opc4DDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc4EDiffPlaces"
 opt.text = "opc4EDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc4FDiffPlaces"
 opt.text = "opc4FDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc5CDiffPlaces"
 opt.text = "opc5CDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc5DDiffPlaces"
 opt.text = "opc5DDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc5EDiffPlaces"
 opt.text = "opc5EDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opc5FDiffPlaces"
 opt.text = "opc5FDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcA5DiffPlaces"
 opt.text = "opcA5DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcA6DiffPlaces"
 opt.text = "opcA6DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcA7DiffPlaces"
 opt.text = "opcA7DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcA8DiffPlaces"
 opt.text = "opcA8DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcA9DiffPlaces"
 opt.text = "opcA9DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcAADiffPlaces"
 opt.text = "opcAADiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcABDiffPlaces"
 opt.text = "opcABDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcACDiffPlaces"
 opt.text = "opcACDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcADDiffPlaces"
 opt.text = "opcADDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcAEDiffPlaces"
 opt.text = "opcAEDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcAFDiffPlaces"
 opt.text = "opcAFDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB0DiffPlaces"
 opt.text = "opcB0DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB1DiffPlaces"
 opt.text = "opcB1DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB2DiffPlaces"
 opt.text = "opcB2DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB3DiffPlaces"
 opt.text = "opcB3DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB4DiffPlaces"
 opt.text = "opcB4DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB5DiffPlaces"
 opt.text = "opcB5DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB6DiffPlaces"
 opt.text = "opcB6DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB7DiffPlaces"
 opt.text = "opcB7DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB8DiffPlaces"
 opt.text = "opcB8DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcB9DiffPlaces"
 opt.text = "opcB9DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcBADiffPlaces"
 opt.text = "opcBADiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcBBDiffPlaces"
 opt.text = "opcBBDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcBCDiffPlaces"
 opt.text = "opcBCDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcBDDiffPlaces"
 opt.text = "opcBDDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcBEDiffPlaces"
 opt.text = "opcBEDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcBFDiffPlaces"
 opt.text = "opcBFDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC0DiffPlaces"
 opt.text = "opcC0DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC1DiffPlaces"
 opt.text = "opcC1DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC2DiffPlaces"
 opt.text = "opcC2DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC3DiffPlaces"
 opt.text = "opcC3DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC4DiffPlaces"
 opt.text = "opcC4DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC5DiffPlaces"
 opt.text = "opcC5DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC6DiffPlaces"
 opt.text = "opcC6DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC7DiffPlaces"
 opt.text = "opcC7DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC8DiffPlaces"
 opt.text = "opcC8DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcC9DiffPlaces"
 opt.text = "opcC9DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcCADiffPlaces"
 opt.text = "opcCADiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcCBDiffPlaces"
 opt.text = "opcCBDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcCCDiffPlaces"
 opt.text = "opcCCDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcCDDiffPlaces"
 opt.text = "opcCDDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcCEDiffPlaces"
 opt.text = "opcCEDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcCFDiffPlaces"
 opt.text = "opcCFDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD0DiffPlaces"
 opt.text = "opcD0DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD1DiffPlaces"
 opt.text = "opcD1DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD2DiffPlaces"
 opt.text = "opcD2DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD3DiffPlaces"
 opt.text = "opcD3DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD4DiffPlaces"
 opt.text = "opcD4DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD5DiffPlaces"
 opt.text = "opcD5DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD6DiffPlaces"
 opt.text = "opcD6DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD7DiffPlaces"
 opt.text = "opcD7DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD8DiffPlaces"
 opt.text = "opcD8DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcD9DiffPlaces"
 opt.text = "opcD9DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcDADiffPlaces"
 opt.text = "opcDADiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcDBDiffPlaces"
 opt.text = "opcDBDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcDCDiffPlaces"
 opt.text = "opcDCDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcDDDiffPlaces"
 opt.text = "opcDDDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcDEDiffPlaces"
 opt.text = "opcDEDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcDFDiffPlaces"
 opt.text = "opcDFDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE0DiffPlaces"
 opt.text = "opcE0DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE1DiffPlaces"
 opt.text = "opcE1DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE2DiffPlaces"
 opt.text = "opcE2DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE3DiffPlaces"
 opt.text = "opcE3DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE4DiffPlaces"
 opt.text = "opcE4DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE5DiffPlaces"
 opt.text = "opcE5DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE6DiffPlaces"
 opt.text = "opcE6DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE7DiffPlaces"
 opt.text = "opcE7DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE8DiffPlaces"
 opt.text = "opcE8DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcE9DiffPlaces"
 opt.text = "opcE9DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcEADiffPlaces"
 opt.text = "opcEADiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcEBDiffPlaces"
 opt.text = "opcEBDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcECDiffPlaces"
 opt.text = "opcECDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcEDDiffPlaces"
 opt.text = "opcEDDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcEEDiffPlaces"
 opt.text = "opcEEDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcEFDiffPlaces"
 opt.text = "opcEFDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcF6DiffPlaces"
 opt.text = "opcF6DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcF7DiffPlaces"
 opt.text = "opcF7DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcF8DiffPlaces"
 opt.text = "opcF8DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcF9DiffPlaces"
 opt.text = "opcF9DiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcFBDiffPlaces"
 opt.text = "opcFBDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcFCDiffPlaces"
 opt.text = "opcFCDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "opcFEDiffPlaces"
 opt.text = "opcFEDiffPlaces"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "operationDiffGas"
 opt.text = "operationDiffGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "undefinedOpcodeFirstByte"
 opt.text = "undefinedOpcodeFirstByte"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stZeroKnowledge2") { 
 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_21000_0"
 opt.text = "ecadd_0-0_0-0_21000_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_21000_128"
 opt.text = "ecadd_0-0_0-0_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_21000_192"
 opt.text = "ecadd_0-0_0-0_21000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_21000_64"
 opt.text = "ecadd_0-0_0-0_21000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_21000_80"
 opt.text = "ecadd_0-0_0-0_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_25000_0"
 opt.text = "ecadd_0-0_0-0_25000_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_25000_128"
 opt.text = "ecadd_0-0_0-0_25000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_25000_192"
 opt.text = "ecadd_0-0_0-0_25000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_25000_64"
 opt.text = "ecadd_0-0_0-0_25000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_0-0_25000_80"
 opt.text = "ecadd_0-0_0-0_25000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_1-2_21000_128"
 opt.text = "ecadd_0-0_1-2_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_1-2_21000_192"
 opt.text = "ecadd_0-0_1-2_21000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_1-2_25000_128"
 opt.text = "ecadd_0-0_1-2_25000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_1-2_25000_192"
 opt.text = "ecadd_0-0_1-2_25000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_1-3_21000_128"
 opt.text = "ecadd_0-0_1-3_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-0_1-3_25000_128"
 opt.text = "ecadd_0-0_1-3_25000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-3_1-2_21000_128"
 opt.text = "ecadd_0-3_1-2_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_0-3_1-2_25000_128"
 opt.text = "ecadd_0-3_1-2_25000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_0-0_21000_128"
 opt.text = "ecadd_1-2_0-0_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_0-0_21000_192"
 opt.text = "ecadd_1-2_0-0_21000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_0-0_21000_64"
 opt.text = "ecadd_1-2_0-0_21000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_0-0_25000_128"
 opt.text = "ecadd_1-2_0-0_25000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_0-0_25000_192"
 opt.text = "ecadd_1-2_0-0_25000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_0-0_25000_64"
 opt.text = "ecadd_1-2_0-0_25000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_1-2_21000_128"
 opt.text = "ecadd_1-2_1-2_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_1-2_21000_192"
 opt.text = "ecadd_1-2_1-2_21000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_1-2_25000_128"
 opt.text = "ecadd_1-2_1-2_25000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-2_1-2_25000_192"
 opt.text = "ecadd_1-2_1-2_25000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-3_0-0_21000_80"
 opt.text = "ecadd_1-3_0-0_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1-3_0-0_25000_80"
 opt.text = "ecadd_1-3_0-0_25000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1145-3932_1145-4651_21000_192"
 opt.text = "ecadd_1145-3932_1145-4651_21000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1145-3932_1145-4651_25000_192"
 opt.text = "ecadd_1145-3932_1145-4651_25000_192"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1145-3932_2969-1336_21000_128"
 opt.text = "ecadd_1145-3932_2969-1336_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_1145-3932_2969-1336_25000_128"
 opt.text = "ecadd_1145-3932_2969-1336_25000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_6-9_19274124-124124_21000_128"
 opt.text = "ecadd_6-9_19274124-124124_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecadd_6-9_19274124-124124_25000_128"
 opt.text = "ecadd_6-9_19274124-124124_25000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_21000_0"
 opt.text = "ecmul_0-0_0_21000_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_21000_128"
 opt.text = "ecmul_0-0_0_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_21000_40"
 opt.text = "ecmul_0-0_0_21000_40"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_21000_64"
 opt.text = "ecmul_0-0_0_21000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_21000_80"
 opt.text = "ecmul_0-0_0_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_21000_96"
 opt.text = "ecmul_0-0_0_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_28000_0"
 opt.text = "ecmul_0-0_0_28000_0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_28000_128"
 opt.text = "ecmul_0-0_0_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_28000_40"
 opt.text = "ecmul_0-0_0_28000_40"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_28000_64"
 opt.text = "ecmul_0-0_0_28000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_28000_80"
 opt.text = "ecmul_0-0_0_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_0_28000_96"
 opt.text = "ecmul_0-0_0_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_1_21000_128"
 opt.text = "ecmul_0-0_1_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_1_21000_96"
 opt.text = "ecmul_0-0_1_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_1_28000_128"
 opt.text = "ecmul_0-0_1_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_1_28000_96"
 opt.text = "ecmul_0-0_1_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_2_21000_128"
 opt.text = "ecmul_0-0_2_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_2_21000_96"
 opt.text = "ecmul_0-0_2_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_2_28000_128"
 opt.text = "ecmul_0-0_2_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_2_28000_96"
 opt.text = "ecmul_0-0_2_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_340282366920938463463374607431768211456_21000_128"
 opt.text = "ecmul_0-0_340282366920938463463374607431768211456_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_340282366920938463463374607431768211456_21000_80"
 opt.text = "ecmul_0-0_340282366920938463463374607431768211456_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_340282366920938463463374607431768211456_21000_96"
 opt.text = "ecmul_0-0_340282366920938463463374607431768211456_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_340282366920938463463374607431768211456_28000_128"
 opt.text = "ecmul_0-0_340282366920938463463374607431768211456_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_340282366920938463463374607431768211456_28000_80"
 opt.text = "ecmul_0-0_340282366920938463463374607431768211456_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_340282366920938463463374607431768211456_28000_96"
 opt.text = "ecmul_0-0_340282366920938463463374607431768211456_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_5616_21000_128"
 opt.text = "ecmul_0-0_5616_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_5616_21000_96"
 opt.text = "ecmul_0-0_5616_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_5616_28000_128"
 opt.text = "ecmul_0-0_5616_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_5616_28000_96"
 opt.text = "ecmul_0-0_5616_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_5617_21000_128"
 opt.text = "ecmul_0-0_5617_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_5617_21000_96"
 opt.text = "ecmul_0-0_5617_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_5617_28000_128"
 opt.text = "ecmul_0-0_5617_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_5617_28000_96"
 opt.text = "ecmul_0-0_5617_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_9935_21000_128"
 opt.text = "ecmul_0-0_9935_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_9935_21000_96"
 opt.text = "ecmul_0-0_9935_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_9935_28000_128"
 opt.text = "ecmul_0-0_9935_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_9935_28000_96"
 opt.text = "ecmul_0-0_9935_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_9_21000_128"
 opt.text = "ecmul_0-0_9_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_9_21000_96"
 opt.text = "ecmul_0-0_9_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_9_28000_128"
 opt.text = "ecmul_0-0_9_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-0_9_28000_96"
 opt.text = "ecmul_0-0_9_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_0_21000_128"
 opt.text = "ecmul_0-3_0_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_0_21000_64"
 opt.text = "ecmul_0-3_0_21000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_0_21000_80"
 opt.text = "ecmul_0-3_0_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_0_21000_96"
 opt.text = "ecmul_0-3_0_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_0_28000_128"
 opt.text = "ecmul_0-3_0_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_0_28000_64"
 opt.text = "ecmul_0-3_0_28000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_0_28000_80"
 opt.text = "ecmul_0-3_0_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_0_28000_96"
 opt.text = "ecmul_0-3_0_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_1_21000_128"
 opt.text = "ecmul_0-3_1_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_1_21000_96"
 opt.text = "ecmul_0-3_1_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_1_28000_128"
 opt.text = "ecmul_0-3_1_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_1_28000_96"
 opt.text = "ecmul_0-3_1_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_2_21000_128"
 opt.text = "ecmul_0-3_2_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_2_21000_96"
 opt.text = "ecmul_0-3_2_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_2_28000_128"
 opt.text = "ecmul_0-3_2_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_2_28000_96"
 opt.text = "ecmul_0-3_2_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_340282366920938463463374607431768211456_21000_128"
 opt.text = "ecmul_0-3_340282366920938463463374607431768211456_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_340282366920938463463374607431768211456_21000_80"
 opt.text = "ecmul_0-3_340282366920938463463374607431768211456_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_340282366920938463463374607431768211456_21000_96"
 opt.text = "ecmul_0-3_340282366920938463463374607431768211456_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_340282366920938463463374607431768211456_28000_128"
 opt.text = "ecmul_0-3_340282366920938463463374607431768211456_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_340282366920938463463374607431768211456_28000_80"
 opt.text = "ecmul_0-3_340282366920938463463374607431768211456_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_340282366920938463463374607431768211456_28000_96"
 opt.text = "ecmul_0-3_340282366920938463463374607431768211456_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_5616_21000_128"
 opt.text = "ecmul_0-3_5616_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_5616_21000_96"
 opt.text = "ecmul_0-3_5616_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_5616_28000_128"
 opt.text = "ecmul_0-3_5616_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_5616_28000_96"
 opt.text = "ecmul_0-3_5616_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_5617_21000_128"
 opt.text = "ecmul_0-3_5617_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_5617_21000_96"
 opt.text = "ecmul_0-3_5617_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_5617_28000_128"
 opt.text = "ecmul_0-3_5617_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_5617_28000_96"
 opt.text = "ecmul_0-3_5617_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_9935_21000_128"
 opt.text = "ecmul_0-3_9935_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_9935_21000_96"
 opt.text = "ecmul_0-3_9935_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_9935_28000_128"
 opt.text = "ecmul_0-3_9935_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_9935_28000_96"
 opt.text = "ecmul_0-3_9935_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_9_21000_128"
 opt.text = "ecmul_0-3_9_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_9_21000_96"
 opt.text = "ecmul_0-3_9_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_9_28000_128"
 opt.text = "ecmul_0-3_9_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_0-3_9_28000_96"
 opt.text = "ecmul_0-3_9_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_0_21000_128"
 opt.text = "ecmul_1-2_0_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_0_21000_64"
 opt.text = "ecmul_1-2_0_21000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_0_21000_80"
 opt.text = "ecmul_1-2_0_21000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_0_21000_96"
 opt.text = "ecmul_1-2_0_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_0_28000_128"
 opt.text = "ecmul_1-2_0_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_0_28000_64"
 opt.text = "ecmul_1-2_0_28000_64"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_0_28000_80"
 opt.text = "ecmul_1-2_0_28000_80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_0_28000_96"
 opt.text = "ecmul_1-2_0_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_1_21000_128"
 opt.text = "ecmul_1-2_1_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_1_21000_96"
 opt.text = "ecmul_1-2_1_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_1_28000_128"
 opt.text = "ecmul_1-2_1_28000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_1_28000_96"
 opt.text = "ecmul_1-2_1_28000_96"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_2_21000_128"
 opt.text = "ecmul_1-2_2_21000_128"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ecmul_1-2_2_21000_96"
 opt.text = "ecmul_1-2_2_21000_96"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stStaticCall") { 
 var opt = document.createElement("option")
 opt.value = "StaticcallToPrecompileFromCalledContract"
 opt.text = "StaticcallToPrecompileFromCalledContract"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "StaticcallToPrecompileFromContractInitialization"
 opt.text = "StaticcallToPrecompileFromContractInitialization"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "StaticcallToPrecompileFromTransaction"
 opt.text = "StaticcallToPrecompileFromTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ABAcalls0"
 opt.text = "static_ABAcalls0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ABAcalls1"
 opt.text = "static_ABAcalls1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ABAcalls2"
 opt.text = "static_ABAcalls2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ABAcalls3"
 opt.text = "static_ABAcalls3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ABAcallsSuicide0"
 opt.text = "static_ABAcallsSuicide0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ABAcallsSuicide1"
 opt.text = "static_ABAcallsSuicide1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CALL_OneVCallSuicide"
 opt.text = "static_CALL_OneVCallSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CALL_ZeroVCallSuicide"
 opt.text = "static_CALL_ZeroVCallSuicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CREATE_ContractSuicideDuringInit"
 opt.text = "static_CREATE_ContractSuicideDuringInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CREATE_ContractSuicideDuringInit_ThenStoreThenReturn"
 opt.text = "static_CREATE_ContractSuicideDuringInit_ThenStoreThenReturn"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CREATE_ContractSuicideDuringInit_WithValue"
 opt.text = "static_CREATE_ContractSuicideDuringInit_WithValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CREATE_EmptyContractAndCallIt_0wei"
 opt.text = "static_CREATE_EmptyContractAndCallIt_0wei"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CREATE_EmptyContractWithStorageAndCallIt_0wei"
 opt.text = "static_CREATE_EmptyContractWithStorageAndCallIt_0wei"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call1024BalanceTooLow2"
 opt.text = "static_Call1024BalanceTooLow2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call1024BalanceTooLow"
 opt.text = "static_Call1024BalanceTooLow"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call1024OOG"
 opt.text = "static_Call1024OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call1024PreCalls2"
 opt.text = "static_Call1024PreCalls2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call1024PreCalls3"
 opt.text = "static_Call1024PreCalls3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call1024PreCalls"
 opt.text = "static_Call1024PreCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call10"
 opt.text = "static_Call10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call1MB1024Calldepth"
 opt.text = "static_Call1MB1024Calldepth"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000"
 opt.text = "static_Call50000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000_ecrec"
 opt.text = "static_Call50000_ecrec"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000_identity2"
 opt.text = "static_Call50000_identity2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000_identity"
 opt.text = "static_Call50000_identity"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000_rip160"
 opt.text = "static_Call50000_rip160"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000bytesContract50_1"
 opt.text = "static_Call50000bytesContract50_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000bytesContract50_2"
 opt.text = "static_Call50000bytesContract50_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000bytesContract50_3"
 opt.text = "static_Call50000bytesContract50_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallAndCallcodeConsumeMoreGasThenTransactionHas"
 opt.text = "static_CallAndCallcodeConsumeMoreGasThenTransactionHas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallAskMoreGasOnDepth2ThenTransactionHas"
 opt.text = "static_CallAskMoreGasOnDepth2ThenTransactionHas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallContractToCreateContractAndCallItOOG"
 opt.text = "static_CallContractToCreateContractAndCallItOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallContractToCreateContractOOGBonusGas"
 opt.text = "static_CallContractToCreateContractOOGBonusGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallContractToCreateContractOOG"
 opt.text = "static_CallContractToCreateContractOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallContractToCreateContractWhichWouldCreateContractIfCalled"
 opt.text = "static_CallContractToCreateContractWhichWouldCreateContractIfCalled"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover0"
 opt.text = "static_CallEcrecover0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover0_0input"
 opt.text = "static_CallEcrecover0_0input"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover0_Gas2999"
 opt.text = "static_CallEcrecover0_Gas2999"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover0_NoGas"
 opt.text = "static_CallEcrecover0_NoGas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover0_completeReturnValue"
 opt.text = "static_CallEcrecover0_completeReturnValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover0_gas3000"
 opt.text = "static_CallEcrecover0_gas3000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover0_overlappingInputOutput"
 opt.text = "static_CallEcrecover0_overlappingInputOutput"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover1"
 opt.text = "static_CallEcrecover1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover2"
 opt.text = "static_CallEcrecover2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover3"
 opt.text = "static_CallEcrecover3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecover80"
 opt.text = "static_CallEcrecover80"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecoverCheckLength"
 opt.text = "static_CallEcrecoverCheckLength"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecoverCheckLengthWrongV"
 opt.text = "static_CallEcrecoverCheckLengthWrongV"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecoverH_prefixed0"
 opt.text = "static_CallEcrecoverH_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecoverR_prefixed0"
 opt.text = "static_CallEcrecoverR_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecoverS_prefixed0"
 opt.text = "static_CallEcrecoverS_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallEcrecoverV_prefixed0"
 opt.text = "static_CallEcrecoverV_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallGoesOOGOnSecondLevel2"
 opt.text = "static_CallGoesOOGOnSecondLevel2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallGoesOOGOnSecondLevel"
 opt.text = "static_CallGoesOOGOnSecondLevel"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallIdentitiy_1"
 opt.text = "static_CallIdentitiy_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallIdentity_1_nonzeroValue"
 opt.text = "static_CallIdentity_1_nonzeroValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallIdentity_2"
 opt.text = "static_CallIdentity_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallIdentity_3"
 opt.text = "static_CallIdentity_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallIdentity_4"
 opt.text = "static_CallIdentity_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallIdentity_4_gas17"
 opt.text = "static_CallIdentity_4_gas17"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallIdentity_4_gas18"
 opt.text = "static_CallIdentity_4_gas18"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallIdentity_5"
 opt.text = "static_CallIdentity_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallLoseGasOOG"
 opt.text = "static_CallLoseGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBomb0"
 opt.text = "static_CallRecursiveBomb0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBomb0_OOG_atMaxCallDepth"
 opt.text = "static_CallRecursiveBomb0_OOG_atMaxCallDepth"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBomb1"
 opt.text = "static_CallRecursiveBomb1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBomb2"
 opt.text = "static_CallRecursiveBomb2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBomb3"
 opt.text = "static_CallRecursiveBomb3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBombLog2"
 opt.text = "static_CallRecursiveBombLog2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBombLog"
 opt.text = "static_CallRecursiveBombLog"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBombPreCall2"
 opt.text = "static_CallRecursiveBombPreCall2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRecursiveBombPreCall"
 opt.text = "static_CallRecursiveBombPreCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRipemd160_1"
 opt.text = "static_CallRipemd160_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRipemd160_2"
 opt.text = "static_CallRipemd160_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRipemd160_3"
 opt.text = "static_CallRipemd160_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRipemd160_3_postfixed0"
 opt.text = "static_CallRipemd160_3_postfixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRipemd160_3_prefixed0"
 opt.text = "static_CallRipemd160_3_prefixed0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRipemd160_4"
 opt.text = "static_CallRipemd160_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRipemd160_4_gas719"
 opt.text = "static_CallRipemd160_4_gas719"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallRipemd160_5"
 opt.text = "static_CallRipemd160_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_1"
 opt.text = "static_CallSha256_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_1_nonzeroValue"
 opt.text = "static_CallSha256_1_nonzeroValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_2"
 opt.text = "static_CallSha256_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_3"
 opt.text = "static_CallSha256_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_3_postfix0"
 opt.text = "static_CallSha256_3_postfix0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_3_prefix0"
 opt.text = "static_CallSha256_3_prefix0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_4"
 opt.text = "static_CallSha256_4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_4_gas99"
 opt.text = "static_CallSha256_4_gas99"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallSha256_5"
 opt.text = "static_CallSha256_5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallToNameRegistrator0"
 opt.text = "static_CallToNameRegistrator0"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CallToReturn1"
 opt.text = "static_CallToReturn1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CalltoReturn2"
 opt.text = "static_CalltoReturn2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CheckCallCostOOG"
 opt.text = "static_CheckCallCostOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CheckOpcodes2"
 opt.text = "static_CheckOpcodes2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CheckOpcodes3"
 opt.text = "static_CheckOpcodes3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CheckOpcodes4"
 opt.text = "static_CheckOpcodes4"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CheckOpcodes5"
 opt.text = "static_CheckOpcodes5"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_CheckOpcodes"
 opt.text = "static_CheckOpcodes"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ExecuteCallThatAskForeGasThenTrabsactionHas"
 opt.text = "static_ExecuteCallThatAskForeGasThenTrabsactionHas"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_InternalCallHittingGasLimit2"
 opt.text = "static_InternalCallHittingGasLimit2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_InternalCallHittingGasLimit"
 opt.text = "static_InternalCallHittingGasLimit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_InternlCallStoreClearsOOG"
 opt.text = "static_InternlCallStoreClearsOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_LoopCallsDepthThenRevert2"
 opt.text = "static_LoopCallsDepthThenRevert2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_LoopCallsDepthThenRevert3"
 opt.text = "static_LoopCallsDepthThenRevert3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_LoopCallsDepthThenRevert"
 opt.text = "static_LoopCallsDepthThenRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_LoopCallsThenRevert"
 opt.text = "static_LoopCallsThenRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_PostToReturn1"
 opt.text = "static_PostToReturn1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_RETURN_Bounds"
 opt.text = "static_RETURN_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_RETURN_BoundsOOG"
 opt.text = "static_RETURN_BoundsOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_RawCallGasAsk"
 opt.text = "static_RawCallGasAsk"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Return50000_2"
 opt.text = "static_Return50000_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ReturnTest2"
 opt.text = "static_ReturnTest2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ReturnTest"
 opt.text = "static_ReturnTest"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_RevertDepth2"
 opt.text = "static_RevertDepth2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_RevertOpcodeCalls"
 opt.text = "static_RevertOpcodeCalls"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ZeroValue_CALL_OOGRevert"
 opt.text = "static_ZeroValue_CALL_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_ZeroValue_SUICIDE_OOGRevert"
 opt.text = "static_ZeroValue_SUICIDE_OOGRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callBasic"
 opt.text = "static_callBasic"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callChangeRevert"
 opt.text = "static_callChangeRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callCreate2"
 opt.text = "static_callCreate2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callCreate3"
 opt.text = "static_callCreate3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callCreate"
 opt.text = "static_callCreate"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callOutput1"
 opt.text = "static_callOutput1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callOutput2"
 opt.text = "static_callOutput2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callOutput3Fail"
 opt.text = "static_callOutput3Fail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callOutput3"
 opt.text = "static_callOutput3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callOutput3partialFail"
 opt.text = "static_callOutput3partialFail"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callOutput3partial"
 opt.text = "static_callOutput3partial"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callToCallCodeOpCodeCheck"
 opt.text = "static_callToCallCodeOpCodeCheck"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callToCallOpCodeCheck"
 opt.text = "static_callToCallOpCodeCheck"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callToDelCallOpCodeCheck"
 opt.text = "static_callToDelCallOpCodeCheck"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callToStaticOpCodeCheck"
 opt.text = "static_callToStaticOpCodeCheck"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callWithHighValueAndGasOOG"
 opt.text = "static_callWithHighValueAndGasOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callWithHighValueAndOOGatTxLevel"
 opt.text = "static_callWithHighValueAndOOGatTxLevel"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callWithHighValue"
 opt.text = "static_callWithHighValue"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callWithHighValueOOGinCall"
 opt.text = "static_callWithHighValueOOGinCall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_call_OOG_additionalGasCosts1"
 opt.text = "static_call_OOG_additionalGasCosts1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_call_OOG_additionalGasCosts2"
 opt.text = "static_call_OOG_additionalGasCosts2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_call_value_inherit"
 opt.text = "static_call_value_inherit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_call_value_inherit_from_call"
 opt.text = "static_call_value_inherit_from_call"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcall_00"
 opt.text = "static_callcall_00"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcall_00_OOGE"
 opt.text = "static_callcall_00_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcall_00_OOGE_1"
 opt.text = "static_callcall_00_OOGE_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcall_00_OOGE_2"
 opt.text = "static_callcall_00_OOGE_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcall_00_SuicideEnd"
 opt.text = "static_callcall_00_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcall_000"
 opt.text = "static_callcallcall_000"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcall_000_OOGE"
 opt.text = "static_callcallcall_000_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcall_000_OOGMAfter2"
 opt.text = "static_callcallcall_000_OOGMAfter2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcall_000_OOGMAfter"
 opt.text = "static_callcallcall_000_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcall_000_OOGMBefore"
 opt.text = "static_callcallcall_000_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcall_000_SuicideEnd"
 opt.text = "static_callcallcall_000_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcall_000_SuicideMiddle"
 opt.text = "static_callcallcall_000_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcall_ABCB_RECURSIVE"
 opt.text = "static_callcallcall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001"
 opt.text = "static_callcallcallcode_001"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_2"
 opt.text = "static_callcallcallcode_001_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_OOGE"
 opt.text = "static_callcallcallcode_001_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_OOGE_2"
 opt.text = "static_callcallcallcode_001_OOGE_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_OOGMAfter2"
 opt.text = "static_callcallcallcode_001_OOGMAfter2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_OOGMAfter"
 opt.text = "static_callcallcallcode_001_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_OOGMAfter_2"
 opt.text = "static_callcallcallcode_001_OOGMAfter_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_OOGMAfter_3"
 opt.text = "static_callcallcallcode_001_OOGMAfter_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_OOGMBefore2"
 opt.text = "static_callcallcallcode_001_OOGMBefore2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_OOGMBefore"
 opt.text = "static_callcallcallcode_001_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_SuicideEnd2"
 opt.text = "static_callcallcallcode_001_SuicideEnd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_SuicideEnd"
 opt.text = "static_callcallcallcode_001_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_SuicideMiddle2"
 opt.text = "static_callcallcallcode_001_SuicideMiddle2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_001_SuicideMiddle"
 opt.text = "static_callcallcallcode_001_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_ABCB_RECURSIVE2"
 opt.text = "static_callcallcallcode_ABCB_RECURSIVE2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcallcode_ABCB_RECURSIVE"
 opt.text = "static_callcallcallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcode_01_2"
 opt.text = "static_callcallcode_01_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcode_01_OOGE_2"
 opt.text = "static_callcallcode_01_OOGE_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcode_01_SuicideEnd2"
 opt.text = "static_callcallcode_01_SuicideEnd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcode_01_SuicideEnd"
 opt.text = "static_callcallcode_01_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010"
 opt.text = "static_callcallcodecall_010"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_2"
 opt.text = "static_callcallcodecall_010_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_OOGE"
 opt.text = "static_callcallcodecall_010_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_OOGE_2"
 opt.text = "static_callcallcodecall_010_OOGE_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_OOGMAfter2"
 opt.text = "static_callcallcodecall_010_OOGMAfter2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_OOGMAfter"
 opt.text = "static_callcallcodecall_010_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_OOGMAfter_2"
 opt.text = "static_callcallcodecall_010_OOGMAfter_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_OOGMAfter_3"
 opt.text = "static_callcallcodecall_010_OOGMAfter_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_OOGMBefore2"
 opt.text = "static_callcallcodecall_010_OOGMBefore2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_OOGMBefore"
 opt.text = "static_callcallcodecall_010_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_SuicideEnd2"
 opt.text = "static_callcallcodecall_010_SuicideEnd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_SuicideEnd"
 opt.text = "static_callcallcodecall_010_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_SuicideMiddle2"
 opt.text = "static_callcallcodecall_010_SuicideMiddle2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_010_SuicideMiddle"
 opt.text = "static_callcallcodecall_010_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_ABCB_RECURSIVE2"
 opt.text = "static_callcallcodecall_ABCB_RECURSIVE2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecall_ABCB_RECURSIVE"
 opt.text = "static_callcallcodecall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011"
 opt.text = "static_callcallcodecallcode_011"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_2"
 opt.text = "static_callcallcodecallcode_011_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_OOGE"
 opt.text = "static_callcallcodecallcode_011_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_OOGE_2"
 opt.text = "static_callcallcodecallcode_011_OOGE_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_OOGMAfter2"
 opt.text = "static_callcallcodecallcode_011_OOGMAfter2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_OOGMAfter"
 opt.text = "static_callcallcodecallcode_011_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_OOGMAfter_1"
 opt.text = "static_callcallcodecallcode_011_OOGMAfter_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_OOGMAfter_2"
 opt.text = "static_callcallcodecallcode_011_OOGMAfter_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_OOGMBefore2"
 opt.text = "static_callcallcodecallcode_011_OOGMBefore2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_OOGMBefore"
 opt.text = "static_callcallcodecallcode_011_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_SuicideEnd2"
 opt.text = "static_callcallcodecallcode_011_SuicideEnd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_SuicideEnd"
 opt.text = "static_callcallcodecallcode_011_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_SuicideMiddle2"
 opt.text = "static_callcallcodecallcode_011_SuicideMiddle2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_011_SuicideMiddle"
 opt.text = "static_callcallcodecallcode_011_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_ABCB_RECURSIVE2"
 opt.text = "static_callcallcodecallcode_ABCB_RECURSIVE2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcallcodecallcode_ABCB_RECURSIVE"
 opt.text = "static_callcallcodecallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcode_checkPC"
 opt.text = "static_callcode_checkPC"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecall_10"
 opt.text = "static_callcodecall_10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecall_10_2"
 opt.text = "static_callcodecall_10_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecall_10_OOGE"
 opt.text = "static_callcodecall_10_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecall_10_OOGE_2"
 opt.text = "static_callcodecall_10_OOGE_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecall_10_SuicideEnd2"
 opt.text = "static_callcodecall_10_SuicideEnd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecall_10_SuicideEnd"
 opt.text = "static_callcodecall_10_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100"
 opt.text = "static_callcodecallcall_100"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_2"
 opt.text = "static_callcodecallcall_100_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_OOGE2"
 opt.text = "static_callcodecallcall_100_OOGE2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_OOGE"
 opt.text = "static_callcodecallcall_100_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_OOGMAfter2"
 opt.text = "static_callcodecallcall_100_OOGMAfter2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_OOGMAfter"
 opt.text = "static_callcodecallcall_100_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_OOGMAfter_2"
 opt.text = "static_callcodecallcall_100_OOGMAfter_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_OOGMAfter_3"
 opt.text = "static_callcodecallcall_100_OOGMAfter_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_OOGMBefore2"
 opt.text = "static_callcodecallcall_100_OOGMBefore2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_OOGMBefore"
 opt.text = "static_callcodecallcall_100_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_SuicideEnd2"
 opt.text = "static_callcodecallcall_100_SuicideEnd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_SuicideEnd"
 opt.text = "static_callcodecallcall_100_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_SuicideMiddle2"
 opt.text = "static_callcodecallcall_100_SuicideMiddle2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_100_SuicideMiddle"
 opt.text = "static_callcodecallcall_100_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_ABCB_RECURSIVE2"
 opt.text = "static_callcodecallcall_ABCB_RECURSIVE2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcall_ABCB_RECURSIVE"
 opt.text = "static_callcodecallcall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101"
 opt.text = "static_callcodecallcallcode_101"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_2"
 opt.text = "static_callcodecallcallcode_101_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_OOGE"
 opt.text = "static_callcodecallcallcode_101_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_OOGE_2"
 opt.text = "static_callcodecallcallcode_101_OOGE_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_OOGMAfter2"
 opt.text = "static_callcodecallcallcode_101_OOGMAfter2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_OOGMAfter"
 opt.text = "static_callcodecallcallcode_101_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_OOGMAfter_1"
 opt.text = "static_callcodecallcallcode_101_OOGMAfter_1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_OOGMAfter_3"
 opt.text = "static_callcodecallcallcode_101_OOGMAfter_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_OOGMBefore2"
 opt.text = "static_callcodecallcallcode_101_OOGMBefore2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_OOGMBefore"
 opt.text = "static_callcodecallcallcode_101_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_SuicideEnd2"
 opt.text = "static_callcodecallcallcode_101_SuicideEnd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_SuicideEnd"
 opt.text = "static_callcodecallcallcode_101_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_SuicideMiddle2"
 opt.text = "static_callcodecallcallcode_101_SuicideMiddle2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_101_SuicideMiddle"
 opt.text = "static_callcodecallcallcode_101_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_ABCB_RECURSIVE2"
 opt.text = "static_callcodecallcallcode_ABCB_RECURSIVE2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcallcode_ABCB_RECURSIVE"
 opt.text = "static_callcodecallcallcode_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_1102"
 opt.text = "static_callcodecallcodecall_1102"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110"
 opt.text = "static_callcodecallcodecall_110"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_2"
 opt.text = "static_callcodecallcodecall_110_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_OOGE2"
 opt.text = "static_callcodecallcodecall_110_OOGE2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_OOGE"
 opt.text = "static_callcodecallcodecall_110_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_OOGMAfter2"
 opt.text = "static_callcodecallcodecall_110_OOGMAfter2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_OOGMAfter"
 opt.text = "static_callcodecallcodecall_110_OOGMAfter"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_OOGMAfter_2"
 opt.text = "static_callcodecallcodecall_110_OOGMAfter_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_OOGMAfter_3"
 opt.text = "static_callcodecallcodecall_110_OOGMAfter_3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_OOGMBefore2"
 opt.text = "static_callcodecallcodecall_110_OOGMBefore2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_OOGMBefore"
 opt.text = "static_callcodecallcodecall_110_OOGMBefore"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_SuicideEnd2"
 opt.text = "static_callcodecallcodecall_110_SuicideEnd2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_SuicideEnd"
 opt.text = "static_callcodecallcodecall_110_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_SuicideMiddle2"
 opt.text = "static_callcodecallcodecall_110_SuicideMiddle2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_110_SuicideMiddle"
 opt.text = "static_callcodecallcodecall_110_SuicideMiddle"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_ABCB_RECURSIVE2"
 opt.text = "static_callcodecallcodecall_ABCB_RECURSIVE2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecall_ABCB_RECURSIVE"
 opt.text = "static_callcodecallcodecall_ABCB_RECURSIVE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_callcodecallcodecallcode_111_SuicideEnd"
 opt.text = "static_callcodecallcodecallcode_111_SuicideEnd"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_calldelcode_01"
 opt.text = "static_calldelcode_01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_calldelcode_01_OOGE"
 opt.text = "static_calldelcode_01_OOGE"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_contractCreationMakeCallThatAskMoreGasThenTransactionProvided"
 opt.text = "static_contractCreationMakeCallThatAskMoreGasThenTransactionProvided"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_contractCreationOOGdontLeaveEmptyContractViaTransaction"
 opt.text = "static_contractCreationOOGdontLeaveEmptyContractViaTransaction"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log0_emptyMem"
 opt.text = "static_log0_emptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log0_logMemStartTooHigh"
 opt.text = "static_log0_logMemStartTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log0_logMemsizeTooHigh"
 opt.text = "static_log0_logMemsizeTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log0_logMemsizeZero"
 opt.text = "static_log0_logMemsizeZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log0_nonEmptyMem"
 opt.text = "static_log0_nonEmptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log0_nonEmptyMem_logMemSize1"
 opt.text = "static_log0_nonEmptyMem_logMemSize1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log0_nonEmptyMem_logMemSize1_logMemStart31"
 opt.text = "static_log0_nonEmptyMem_logMemSize1_logMemStart31"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log1_MaxTopic"
 opt.text = "static_log1_MaxTopic"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log1_emptyMem"
 opt.text = "static_log1_emptyMem"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log1_logMemStartTooHigh"
 opt.text = "static_log1_logMemStartTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log1_logMemsizeTooHigh"
 opt.text = "static_log1_logMemsizeTooHigh"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log1_logMemsizeZero"
 opt.text = "static_log1_logMemsizeZero"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_log_Caller"
 opt.text = "static_log_Caller"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_makeMoney"
 opt.text = "static_makeMoney"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_refund_CallA"
 opt.text = "static_refund_CallA"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_refund_CallToSuicideNoStorage"
 opt.text = "static_refund_CallToSuicideNoStorage"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_refund_CallToSuicideTwice"
 opt.text = "static_refund_CallToSuicideTwice"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stPreCompiledContracts") { 
 var opt = document.createElement("option")
 opt.value = "blake2B"
 opt.text = "blake2B"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "delegatecall09Undefined"
 opt.text = "delegatecall09Undefined"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "idPrecomps"
 opt.text = "idPrecomps"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "identity_to_bigger"
 opt.text = "identity_to_bigger"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "identity_to_smaller"
 opt.text = "identity_to_smaller"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modexp"
 opt.text = "modexp"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "modexpTests"
 opt.text = "modexpTests"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "precompsEIP2929"
 opt.text = "precompsEIP2929"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sec80"
 opt.text = "sec80"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stCodeCopyTest") { 
 var opt = document.createElement("option")
 opt.value = "ExtCodeCopyTargetRangeLongerThanCodeTests"
 opt.text = "ExtCodeCopyTargetRangeLongerThanCodeTests"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "ExtCodeCopyTests"
 opt.text = "ExtCodeCopyTests"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stTimeConsuming") { 
 var opt = document.createElement("option")
 opt.value = "CALLBlake2f_MaxRounds"
 opt.text = "CALLBlake2f_MaxRounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial00"
 opt.text = "sstore_combinations_initial00"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial00_2"
 opt.text = "sstore_combinations_initial00_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial01"
 opt.text = "sstore_combinations_initial01"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial01_2"
 opt.text = "sstore_combinations_initial01_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial10"
 opt.text = "sstore_combinations_initial10"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial10_2"
 opt.text = "sstore_combinations_initial10_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial11"
 opt.text = "sstore_combinations_initial11"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial11_2"
 opt.text = "sstore_combinations_initial11_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial20"
 opt.text = "sstore_combinations_initial20"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial20_2"
 opt.text = "sstore_combinations_initial20_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial21"
 opt.text = "sstore_combinations_initial21"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "sstore_combinations_initial21_2"
 opt.text = "sstore_combinations_initial21_2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "static_Call50000_sha256"
 opt.text = "static_Call50000_sha256"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stRandom2") { 
 var opt = document.createElement("option")
 opt.value = "201503110226PYTHON_DUP6"
 opt.text = "201503110226PYTHON_DUP6"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest384"
 opt.text = "randomStatetest384"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest385"
 opt.text = "randomStatetest385"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest386"
 opt.text = "randomStatetest386"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest387"
 opt.text = "randomStatetest387"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest388"
 opt.text = "randomStatetest388"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest389"
 opt.text = "randomStatetest389"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest393"
 opt.text = "randomStatetest393"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest395"
 opt.text = "randomStatetest395"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest396"
 opt.text = "randomStatetest396"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest397"
 opt.text = "randomStatetest397"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest398"
 opt.text = "randomStatetest398"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest399"
 opt.text = "randomStatetest399"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest401"
 opt.text = "randomStatetest401"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest402"
 opt.text = "randomStatetest402"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest404"
 opt.text = "randomStatetest404"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest405"
 opt.text = "randomStatetest405"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest406"
 opt.text = "randomStatetest406"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest407"
 opt.text = "randomStatetest407"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest408"
 opt.text = "randomStatetest408"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest409"
 opt.text = "randomStatetest409"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest410"
 opt.text = "randomStatetest410"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest411"
 opt.text = "randomStatetest411"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest412"
 opt.text = "randomStatetest412"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest413"
 opt.text = "randomStatetest413"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest414"
 opt.text = "randomStatetest414"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest415"
 opt.text = "randomStatetest415"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest416"
 opt.text = "randomStatetest416"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest417"
 opt.text = "randomStatetest417"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest418"
 opt.text = "randomStatetest418"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest419"
 opt.text = "randomStatetest419"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest420"
 opt.text = "randomStatetest420"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest421"
 opt.text = "randomStatetest421"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest422"
 opt.text = "randomStatetest422"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest423"
 opt.text = "randomStatetest423"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest424"
 opt.text = "randomStatetest424"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest425"
 opt.text = "randomStatetest425"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest426"
 opt.text = "randomStatetest426"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest428"
 opt.text = "randomStatetest428"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest429"
 opt.text = "randomStatetest429"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest430"
 opt.text = "randomStatetest430"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest433"
 opt.text = "randomStatetest433"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest435"
 opt.text = "randomStatetest435"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest436"
 opt.text = "randomStatetest436"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest437"
 opt.text = "randomStatetest437"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest438"
 opt.text = "randomStatetest438"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest439"
 opt.text = "randomStatetest439"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest440"
 opt.text = "randomStatetest440"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest442"
 opt.text = "randomStatetest442"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest443"
 opt.text = "randomStatetest443"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest444"
 opt.text = "randomStatetest444"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest445"
 opt.text = "randomStatetest445"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest446"
 opt.text = "randomStatetest446"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest447"
 opt.text = "randomStatetest447"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest448"
 opt.text = "randomStatetest448"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest449"
 opt.text = "randomStatetest449"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest450"
 opt.text = "randomStatetest450"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest451"
 opt.text = "randomStatetest451"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest452"
 opt.text = "randomStatetest452"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest454"
 opt.text = "randomStatetest454"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest455"
 opt.text = "randomStatetest455"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest456"
 opt.text = "randomStatetest456"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest457"
 opt.text = "randomStatetest457"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest458"
 opt.text = "randomStatetest458"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest460"
 opt.text = "randomStatetest460"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest461"
 opt.text = "randomStatetest461"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest462"
 opt.text = "randomStatetest462"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest464"
 opt.text = "randomStatetest464"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest465"
 opt.text = "randomStatetest465"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest466"
 opt.text = "randomStatetest466"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest467"
 opt.text = "randomStatetest467"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest468"
 opt.text = "randomStatetest468"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest469"
 opt.text = "randomStatetest469"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest470"
 opt.text = "randomStatetest470"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest471"
 opt.text = "randomStatetest471"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest472"
 opt.text = "randomStatetest472"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest473"
 opt.text = "randomStatetest473"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest474"
 opt.text = "randomStatetest474"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest475"
 opt.text = "randomStatetest475"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest476"
 opt.text = "randomStatetest476"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest477"
 opt.text = "randomStatetest477"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest478"
 opt.text = "randomStatetest478"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest480"
 opt.text = "randomStatetest480"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest481"
 opt.text = "randomStatetest481"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest482"
 opt.text = "randomStatetest482"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest483"
 opt.text = "randomStatetest483"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest484"
 opt.text = "randomStatetest484"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest485"
 opt.text = "randomStatetest485"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest487"
 opt.text = "randomStatetest487"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest488"
 opt.text = "randomStatetest488"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest489"
 opt.text = "randomStatetest489"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest491"
 opt.text = "randomStatetest491"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest493"
 opt.text = "randomStatetest493"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest494"
 opt.text = "randomStatetest494"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest495"
 opt.text = "randomStatetest495"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest496"
 opt.text = "randomStatetest496"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest497"
 opt.text = "randomStatetest497"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest498"
 opt.text = "randomStatetest498"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest499"
 opt.text = "randomStatetest499"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest500"
 opt.text = "randomStatetest500"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest501"
 opt.text = "randomStatetest501"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest502"
 opt.text = "randomStatetest502"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest503"
 opt.text = "randomStatetest503"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest504"
 opt.text = "randomStatetest504"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest505"
 opt.text = "randomStatetest505"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest506"
 opt.text = "randomStatetest506"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest507"
 opt.text = "randomStatetest507"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest508"
 opt.text = "randomStatetest508"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest509"
 opt.text = "randomStatetest509"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest510"
 opt.text = "randomStatetest510"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest511"
 opt.text = "randomStatetest511"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest512"
 opt.text = "randomStatetest512"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest513"
 opt.text = "randomStatetest513"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest514"
 opt.text = "randomStatetest514"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest516"
 opt.text = "randomStatetest516"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest517"
 opt.text = "randomStatetest517"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest518"
 opt.text = "randomStatetest518"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest519"
 opt.text = "randomStatetest519"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest520"
 opt.text = "randomStatetest520"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest521"
 opt.text = "randomStatetest521"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest523"
 opt.text = "randomStatetest523"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest524"
 opt.text = "randomStatetest524"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest525"
 opt.text = "randomStatetest525"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest526"
 opt.text = "randomStatetest526"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest527"
 opt.text = "randomStatetest527"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest528"
 opt.text = "randomStatetest528"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest531"
 opt.text = "randomStatetest531"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest532"
 opt.text = "randomStatetest532"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest533"
 opt.text = "randomStatetest533"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest534"
 opt.text = "randomStatetest534"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest535"
 opt.text = "randomStatetest535"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest536"
 opt.text = "randomStatetest536"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest537"
 opt.text = "randomStatetest537"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest538"
 opt.text = "randomStatetest538"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest539"
 opt.text = "randomStatetest539"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest541"
 opt.text = "randomStatetest541"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest542"
 opt.text = "randomStatetest542"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest543"
 opt.text = "randomStatetest543"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest544"
 opt.text = "randomStatetest544"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest545"
 opt.text = "randomStatetest545"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest546"
 opt.text = "randomStatetest546"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest547"
 opt.text = "randomStatetest547"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest548"
 opt.text = "randomStatetest548"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest550"
 opt.text = "randomStatetest550"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest552"
 opt.text = "randomStatetest552"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest553"
 opt.text = "randomStatetest553"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest554"
 opt.text = "randomStatetest554"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest555"
 opt.text = "randomStatetest555"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest556"
 opt.text = "randomStatetest556"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest558"
 opt.text = "randomStatetest558"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest559"
 opt.text = "randomStatetest559"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest560"
 opt.text = "randomStatetest560"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest562"
 opt.text = "randomStatetest562"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest563"
 opt.text = "randomStatetest563"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest564"
 opt.text = "randomStatetest564"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest565"
 opt.text = "randomStatetest565"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest566"
 opt.text = "randomStatetest566"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest567"
 opt.text = "randomStatetest567"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest569"
 opt.text = "randomStatetest569"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest571"
 opt.text = "randomStatetest571"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest572"
 opt.text = "randomStatetest572"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest573"
 opt.text = "randomStatetest573"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest574"
 opt.text = "randomStatetest574"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest575"
 opt.text = "randomStatetest575"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest576"
 opt.text = "randomStatetest576"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest577"
 opt.text = "randomStatetest577"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest578"
 opt.text = "randomStatetest578"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest579"
 opt.text = "randomStatetest579"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest580"
 opt.text = "randomStatetest580"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest581"
 opt.text = "randomStatetest581"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest582"
 opt.text = "randomStatetest582"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest583"
 opt.text = "randomStatetest583"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest584"
 opt.text = "randomStatetest584"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest585"
 opt.text = "randomStatetest585"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest586"
 opt.text = "randomStatetest586"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest587"
 opt.text = "randomStatetest587"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest588"
 opt.text = "randomStatetest588"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest589"
 opt.text = "randomStatetest589"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest592"
 opt.text = "randomStatetest592"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest596"
 opt.text = "randomStatetest596"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest597"
 opt.text = "randomStatetest597"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest599"
 opt.text = "randomStatetest599"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest600"
 opt.text = "randomStatetest600"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest601"
 opt.text = "randomStatetest601"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest602"
 opt.text = "randomStatetest602"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest603"
 opt.text = "randomStatetest603"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest604"
 opt.text = "randomStatetest604"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest605"
 opt.text = "randomStatetest605"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest607"
 opt.text = "randomStatetest607"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest608"
 opt.text = "randomStatetest608"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest609"
 opt.text = "randomStatetest609"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest610"
 opt.text = "randomStatetest610"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest611"
 opt.text = "randomStatetest611"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest612"
 opt.text = "randomStatetest612"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest615"
 opt.text = "randomStatetest615"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest616"
 opt.text = "randomStatetest616"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest618"
 opt.text = "randomStatetest618"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest620"
 opt.text = "randomStatetest620"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest621"
 opt.text = "randomStatetest621"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest624"
 opt.text = "randomStatetest624"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest625"
 opt.text = "randomStatetest625"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest626"
 opt.text = "randomStatetest626"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest627"
 opt.text = "randomStatetest627"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest628"
 opt.text = "randomStatetest628"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest629"
 opt.text = "randomStatetest629"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest630"
 opt.text = "randomStatetest630"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest632"
 opt.text = "randomStatetest632"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest633"
 opt.text = "randomStatetest633"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest635"
 opt.text = "randomStatetest635"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest636"
 opt.text = "randomStatetest636"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest637"
 opt.text = "randomStatetest637"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest638"
 opt.text = "randomStatetest638"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest639"
 opt.text = "randomStatetest639"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest640"
 opt.text = "randomStatetest640"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest641"
 opt.text = "randomStatetest641"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest642"
 opt.text = "randomStatetest642"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest643"
 opt.text = "randomStatetest643"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest644"
 opt.text = "randomStatetest644"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest645"
 opt.text = "randomStatetest645"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest646"
 opt.text = "randomStatetest646"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest647"
 opt.text = "randomStatetest647"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest648"
 opt.text = "randomStatetest648"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest649"
 opt.text = "randomStatetest649"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest650"
 opt.text = "randomStatetest650"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "randomStatetest"
 opt.text = "randomStatetest"
 document.requesthelper.singletest.add(opt, null) 

 }
 if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == "stCreate2") { 
 var opt = document.createElement("option")
 opt.value = "CREATE2_Bounds2"
 opt.text = "CREATE2_Bounds2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_Bounds3"
 opt.text = "CREATE2_Bounds3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_Bounds"
 opt.text = "CREATE2_Bounds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_ContractSuicideDuringInit_ThenStoreThenReturn"
 opt.text = "CREATE2_ContractSuicideDuringInit_ThenStoreThenReturn"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_EOF1"
 opt.text = "CREATE2_EOF1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_FirstByte_loop"
 opt.text = "CREATE2_FirstByte_loop"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_HighNonceDelegatecall"
 opt.text = "CREATE2_HighNonceDelegatecall"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_HighNonce"
 opt.text = "CREATE2_HighNonce"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_HighNonceMinus1"
 opt.text = "CREATE2_HighNonceMinus1"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CREATE2_Suicide"
 opt.text = "CREATE2_Suicide"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OOGFromCallRefunds"
 opt.text = "Create2OOGFromCallRefunds"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OOGafterInitCode"
 opt.text = "Create2OOGafterInitCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OOGafterInitCodeReturndata2"
 opt.text = "Create2OOGafterInitCodeReturndata2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OOGafterInitCodeReturndata3"
 opt.text = "Create2OOGafterInitCodeReturndata3"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OOGafterInitCodeReturndata"
 opt.text = "Create2OOGafterInitCodeReturndata"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OOGafterInitCodeReturndataSize"
 opt.text = "Create2OOGafterInitCodeReturndataSize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OOGafterInitCodeRevert2"
 opt.text = "Create2OOGafterInitCodeRevert2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OOGafterInitCodeRevert"
 opt.text = "Create2OOGafterInitCodeRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OnDepth1023"
 opt.text = "Create2OnDepth1023"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2OnDepth1024"
 opt.text = "Create2OnDepth1024"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "Create2Recursive"
 opt.text = "Create2Recursive"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateMessageReverted"
 opt.text = "CreateMessageReverted"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "CreateMessageRevertedOOGInInit"
 opt.text = "CreateMessageRevertedOOGInInit"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertDepthCreate2OOGBerlin"
 opt.text = "RevertDepthCreate2OOGBerlin"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertDepthCreate2OOG"
 opt.text = "RevertDepthCreate2OOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertDepthCreateAddressCollisionBerlin"
 opt.text = "RevertDepthCreateAddressCollisionBerlin"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertDepthCreateAddressCollision"
 opt.text = "RevertDepthCreateAddressCollision"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertInCreateInInitCreate2"
 opt.text = "RevertInCreateInInitCreate2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeCreate"
 opt.text = "RevertOpcodeCreate"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "RevertOpcodeInCreateReturnsCreate2"
 opt.text = "RevertOpcodeInCreateReturnsCreate2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "call_outsize_then_create2_successful_then_returndatasize"
 opt.text = "call_outsize_then_create2_successful_then_returndatasize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "call_then_create2_successful_then_returndatasize"
 opt.text = "call_then_create2_successful_then_returndatasize"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2InitCodes"
 opt.text = "create2InitCodes"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2SmartInitCode"
 opt.text = "create2SmartInitCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2callPrecompiles"
 opt.text = "create2callPrecompiles"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2checkFieldsInInitcode"
 opt.text = "create2checkFieldsInInitcode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionBalance"
 opt.text = "create2collisionBalance"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionCode2"
 opt.text = "create2collisionCode2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionCode"
 opt.text = "create2collisionCode"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionNonce"
 opt.text = "create2collisionNonce"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionSelfdestructed2"
 opt.text = "create2collisionSelfdestructed2"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionSelfdestructed"
 opt.text = "create2collisionSelfdestructed"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionSelfdestructedOOG"
 opt.text = "create2collisionSelfdestructedOOG"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionSelfdestructedRevert"
 opt.text = "create2collisionSelfdestructedRevert"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2collisionStorage"
 opt.text = "create2collisionStorage"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "create2noCash"
 opt.text = "create2noCash"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_0_0_following_successful_create"
 opt.text = "returndatacopy_0_0_following_successful_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_afterFailing_create"
 opt.text = "returndatacopy_afterFailing_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_create"
 opt.text = "returndatacopy_following_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_revert_in_create"
 opt.text = "returndatacopy_following_revert_in_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatacopy_following_successful_create"
 opt.text = "returndatacopy_following_successful_create"
 document.requesthelper.singletest.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "returndatasize_following_successful_create"
 opt.text = "returndatasize_following_successful_create"
 document.requesthelper.singletest.add(opt, null) 

 }
}
function testinit() { 
 document.requesthelper.gtests.options.length=0 
 var opt = document.createElement("option")
 opt.value = "stWalletTest"
 opt.text = "stWalletTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stBugs"
 opt.text = "stBugs"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stEIP1559"
 opt.text = "stEIP1559"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stDelegatecallTestHomestead"
 opt.text = "stDelegatecallTestHomestead"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stChainId"
 opt.text = "stChainId"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stExtCodeHash"
 opt.text = "stExtCodeHash"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stZeroCallsRevert"
 opt.text = "stZeroCallsRevert"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stSelfBalance"
 opt.text = "stSelfBalance"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stQuadraticComplexityTest"
 opt.text = "stQuadraticComplexityTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stCodeSizeLimit"
 opt.text = "stCodeSizeLimit"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stReturnDataTest"
 opt.text = "stReturnDataTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stRandom"
 opt.text = "stRandom"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stPreCompiledContracts2"
 opt.text = "stPreCompiledContracts2"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stZeroCallsTest"
 opt.text = "stZeroCallsTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stSStoreTest"
 opt.text = "stSStoreTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stLogTests"
 opt.text = "stLogTests"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stStackTests"
 opt.text = "stStackTests"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stTransitionTest"
 opt.text = "stTransitionTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stZeroKnowledge"
 opt.text = "stZeroKnowledge"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stCallDelegateCodesHomestead"
 opt.text = "stCallDelegateCodesHomestead"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stHomesteadSpecific"
 opt.text = "stHomesteadSpecific"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stExample"
 opt.text = "stExample"
 opt.selected = true
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stSolidityTest"
 opt.text = "stSolidityTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stMemExpandingEIP150Calls"
 opt.text = "stMemExpandingEIP150Calls"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stMemoryStressTest"
 opt.text = "stMemoryStressTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stArgsZeroOneBalance"
 opt.text = "stArgsZeroOneBalance"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stCreateTest"
 opt.text = "stCreateTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stAttackTest"
 opt.text = "stAttackTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stRecursiveCreate"
 opt.text = "stRecursiveCreate"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stShift"
 opt.text = "stShift"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stCallCreateCallCodeTest"
 opt.text = "stCallCreateCallCodeTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stRevertTest"
 opt.text = "stRevertTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stCallCodes"
 opt.text = "stCallCodes"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stEIP158Specific"
 opt.text = "stEIP158Specific"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stCallDelegateCodesCallCodeHomestead"
 opt.text = "stCallDelegateCodesCallCodeHomestead"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stTransactionTest"
 opt.text = "stTransactionTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stEIP150singleCodeGasPrices"
 opt.text = "stEIP150singleCodeGasPrices"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stSLoadTest"
 opt.text = "stSLoadTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stMemoryTest"
 opt.text = "stMemoryTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stEIP3607"
 opt.text = "stEIP3607"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "VMTests"
 opt.text = "VMTests"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stRefundTest"
 opt.text = "stRefundTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stInitCodeTest"
 opt.text = "stInitCodeTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stNonZeroCallsTest"
 opt.text = "stNonZeroCallsTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stSpecialTest"
 opt.text = "stSpecialTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stStaticFlagEnabled"
 opt.text = "stStaticFlagEnabled"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stEIP150Specific"
 opt.text = "stEIP150Specific"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stExpectSection"
 opt.text = "stExpectSection"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stEIP2930"
 opt.text = "stEIP2930"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stSystemOperationsTest"
 opt.text = "stSystemOperationsTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stBadOpcode"
 opt.text = "stBadOpcode"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stZeroKnowledge2"
 opt.text = "stZeroKnowledge2"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stStaticCall"
 opt.text = "stStaticCall"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stPreCompiledContracts"
 opt.text = "stPreCompiledContracts"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stCodeCopyTest"
 opt.text = "stCodeCopyTest"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stTimeConsuming"
 opt.text = "stTimeConsuming"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stRandom2"
 opt.text = "stRandom2"
 document.requesthelper.gtests.add(opt, null) 

 var opt = document.createElement("option")
 opt.value = "stCreate2"
 opt.text = "stCreate2"
 document.requesthelper.gtests.add(opt, null) 

 singletests() 
}

