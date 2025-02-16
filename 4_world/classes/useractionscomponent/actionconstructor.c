class ActionConstructor
{
	bool ConstructActions( out array<ref ActionBase> actionsArray, out map<typename, ActionBase> actionNameActionMap )
	{
		TTypenameArray action_array_names = new TTypenameArray;
		RegisterActions(action_array_names);
		Sort(action_array_names,action_array_names.Count());
		
		actionsArray = new array<ref ActionBase>;
		actionNameActionMap = new map<typename, ActionBase>;
		ActionBase new_action;
		
		for(int i = 0; i < action_array_names.Count(); i++)
		{
			new_action = ActionBase.Cast(action_array_names[i].Spawn());
			if(new_action)
			{
				new_action.CreateConditionComponents();
				new_action.SetID(actionsArray.Count());
				actionsArray.Insert(new_action);
				actionNameActionMap.Insert(action_array_names[i], new_action);
			}
		}
		return true;
	}
	
	void RegisterActions(TTypenameArray actions)
	{
		// SingleUse Actions	
		actions.Insert(ActionOpen);
		actions.Insert(ActionClose);
		actions.Insert(ActionConsumeSingle);
		actions.Insert(ActionForceConsumeSingle);
		actions.Insert(ActionTurnOnChemlight);
		actions.Insert(ActionMeasureTemperatureSelf);
		actions.Insert(ActionMeasureTemperatureTarget);
		actions.Insert(ActionDisinfectSelf);
		actions.Insert(ActionDisinfectTarget);
		actions.Insert(ActionPlugIn);
		actions.Insert(ActionPlugIntoFence);
		actions.Insert(ActionTogglePlaceObject);
		actions.Insert(ActionTurnOnWhileInHands);
		actions.Insert(ActionTurnOffWhileInHands);
		actions.Insert(ActionPlantSeed);
		actions.Insert(ActionDisinfectPlantBit);
		actions.Insert(ActionTurnOnHeatpack);
//		actions.Insert(ActionAttachToCharger);
		actions.Insert(ActionTurnOnTransmitter);	
		actions.Insert(ActionTurnOffTransmitter);
		actions.Insert(ActionWashHandsItem);
		actions.Insert(ActionTakeABiteCan);
		actions.Insert(ActionForceABiteCan);
		actions.Insert(ActionPlaceFireplaceIndoor);
		actions.Insert(ActionPlaceOvenIndoor);
		actions.Insert(ActionCreateIndoorFireplace);
		actions.Insert(ActionCreateIndoorOven);
		actions.Insert(ActionAttach);
		actions.Insert(ActionAttachOnProxy);
		actions.Insert(ActionAttachWheels);
		actions.Insert(ActionPlugTargetIntoThis);
		actions.Insert(ActionInsertSparkplug);
		actions.Insert(ActionEmptySeedsPack);
		actions.Insert(ActionWorldCraftSwitch);
		actions.Insert(ActionWorldLiquidActionSwitch);
		actions.Insert(ActionAttachPowerSourceToPanel);
//		actions.Insert(ActionAttachMetalWire);
		actions.Insert(ActionInjectSelf);
		actions.Insert(ActionInjectTarget);
		actions.Insert(ActionInjectEpinephrineSelf);
		actions.Insert(ActionInjectEpinephrineTarget);
		actions.Insert(ActionInjectMorphineSelf);
		actions.Insert(ActionInjectMorphineTarget);
		actions.Insert(ActionHandcuffTarget);
		actions.Insert(ActionBuildPartSwitch);
		actions.Insert(ActionAttachOnSelection);
		actions.Insert(ActionTurnOnWeaponFlashlight);
		actions.Insert(ActionTurnOffWeaponFlashlight);
		actions.Insert(ActionNextCombinationLockDial);
		actions.Insert(ActionAttachToConstruction);
		actions.Insert(ActionTakeMaterialToHandsSwitch);
		actions.Insert(ActionClapBearTrapWithThisItem);
		actions.Insert(ActionPlaceFireplaceIntoBarrel);
		actions.Insert(ActionStopEngine);
		actions.Insert(ActionUnfoldMap);
		actions.Insert(ActionDropItem);
		actions.Insert(ActionDropItemSimple);
		actions.Insert(ActionSwitchSeats);
		actions.Insert(ActionTurnOnHeadtorch);
		actions.Insert(ActionTurnOffHeadtorch);
		actions.Insert(ActionTurnOnHelmetFlashlight);
		actions.Insert(ActionTurnOffHelmetFlashlight);
		actions.Insert(ActionToggleNVG);

		// Continuous use actions
		actions.Insert(ActionWorldCraft);
//		actions.Insert(ActionDigStash);
		actions.Insert(ActionSkinning);
		actions.Insert(ActionPlaceObject);
		actions.Insert(ActionDeployObject);
		actions.Insert(ActionMineTree);
		actions.Insert(ActionMineBush);
		actions.Insert(ActionMineRock);
		actions.Insert(ActionMineRock1H);
		actions.Insert(ActionDrinkDisinfectant);
		actions.Insert(ActionForceDrinkDisinfectant);	
		actions.Insert(ActionDrinkDisinfectant2);
		actions.Insert(ActionDigGardenPlot);
		actions.Insert(ActionWaterPlant);
		actions.Insert(ActionFertilizeSlot);
		actions.Insert(ActionDisinfectPlant);
		actions.Insert(ActionFillObject);
		actions.Insert(ActionDefibrilateSelf);
		actions.Insert(ActionDefibrilateTarget);
		actions.Insert(ActionTestBloodSelf);
		actions.Insert(ActionTestBloodTarget);
		actions.Insert(ActionGiveBloodSelf);
		actions.Insert(ActionGiveBloodTarget);
		actions.Insert(ActionGiveSalineSelf);
		actions.Insert(ActionGiveSalineTarget);
		actions.Insert(ActionCollectBloodSelf);
		actions.Insert(ActionCollectBloodTarget);
		actions.Insert(ActionCollectSampleSelf);
		actions.Insert(ActionCollectSampleTarget);
		actions.Insert(ActionCoverHeadSelf);
		actions.Insert(ActionUncoverHeadSelf);
		actions.Insert(ActionCoverHeadTarget);
		actions.Insert(ActionShave);
		actions.Insert(ActionSplintSelf);
		actions.Insert(ActionSplintTarget);
		actions.Insert(ActionConsume);
		actions.Insert(ActionForceConsume);
		actions.Insert(ActionForceDrink);
		actions.Insert(ActionForceFeed);
		actions.Insert(ActionEat);
		actions.Insert(ActionEatMeat);
		actions.Insert(ActionEatFruit);
		actions.Insert(ActionDrink);
		actions.Insert(ActionDrinkCookingPot);
		actions.Insert(ActionBandageSelf);
		actions.Insert(ActionBandageTarget);
		actions.Insert(ActionRestrainTarget);
		actions.Insert(ActionRestrainSelf);
		actions.Insert(ActionUnrestrainTarget);
		actions.Insert(ActionUnrestrainSelf);
		actions.Insert(ActionPlaceOnGround);
		actions.Insert(ActionForceDrinkAlcohol);
		actions.Insert(ActionDrinkAlcohol);
		//actions.Insert(ActionFishing);
		actions.Insert(ActionFillBottleBase);
		actions.Insert(ActionUpgradeTorchFromGasPump);
		actions.Insert(ActionTuneFrequency);
		actions.Insert(ActionTuneRadioStation);
		actions.Insert(ActionIgniteFireplaceByAir);
		actions.Insert(ActionLightItemOnFire);
		actions.Insert(ActionExtinguishFireplaceByLiquid);
		actions.Insert(ActionExtinguishFireplaceByExtinguisher);
		actions.Insert(ActionBurnSewSelf);
		actions.Insert(ActionBurnSewTarget);
		actions.Insert(ActionSewSelf);
		actions.Insert(ActionSewTarget);
		actions.Insert(ActionBuryAshes);
		actions.Insert(ActionCookOnStick);
		actions.Insert(ActionFillGeneratorTank);
		actions.Insert(ActionSawPlanks);
		actions.Insert(ActionBuryBody);
		actions.Insert(ActionEatCan);
		actions.Insert(ActionForceFeedCan);
		actions.Insert(ActionForceFeedMeat);
		actions.Insert(ActionWaterGardenSlot);
		actions.Insert(ActionWringClothes);
		actions.Insert(ActionLoadMagazine);
		actions.Insert(ActionEmptyMagazine);
		actions.Insert(ActionBuildPart);
		actions.Insert(ActionDismantlePart);
		actions.Insert(ActionDestroyPart);
		actions.Insert(ActionCPR);
		actions.Insert(ActionDrinkPondContinuous);
		actions.Insert(ActionEatSmall);
		actions.Insert(ActionMineTreeBark);
		actions.Insert(ActionDigWorms);
		actions.Insert(ActionShaveTarget);
		actions.Insert(ActionDrinkWellContinuous);
		actions.Insert(ActionUncoverHeadTarget);
		actions.Insert(ActionLockDoors);
		actions.Insert(ActionUnlockDoors);
		actions.Insert(ActionUnpackBox);
		//actions.Insert(ActionWashHandsWater);
		//actions.Insert(ActionWashHandsWell);
		actions.Insert(ActionDrinkCan);
		actions.Insert(ActionStripCarrierVest);
		actions.Insert(ActionBuildOven);
		actions.Insert(ActionDismantleOven);
		actions.Insert(ActionBuildStoneCircle);
		actions.Insert(ActionDismantleStoneCircle);
		//actions.Insert(ActionWritePaper);
		//actions.Insert(ActionReadPaper);
		actions.Insert(ActionFillFuel);
		actions.Insert(ActionTransferLiquid);
		actions.Insert(ActionStartEngine);
		actions.Insert(ActionRaiseMegaphone);
		actions.Insert(ActionActivateTrap);
		actions.Insert(ActionEmptyBottleBase);
		actions.Insert(ActionEmptyCookingPot);
		actions.Insert(ActionViewOptics);
		actions.Insert(ActionViewBinoculars);
		actions.Insert(ActionFoldBaseBuildingObject);
		//actions.Insert(ActionUseRangefinder);
		//actions.Insert(ActionAttached);
		actions.Insert(ActionMountBarbedWire);
		actions.Insert(ActionUnmountBarbedWire);
		actions.Insert(ActionDialCombinationLock);
		actions.Insert(ActionDialCombinationLockOnTarget);
		actions.Insert(ActionFillCoolant);
		actions.Insert(ActionFillOil);
		actions.Insert(ActionDestroyCombinationLock);
		actions.Insert(ActionGagSelf);
		actions.Insert(ActionGagTarget);
		actions.Insert(ActionUngagSelf);
		actions.Insert(ActionUngagTarget);
		actions.Insert(ActionDigInStash);
		actions.Insert(ActionDigOutStash);
		actions.Insert(ActionMineBushByHand);
		actions.Insert(ActionBreakLongWoodenStick);
		actions.Insert(ActionFishingNew);
		actions.Insert(ActionRepairTent);
		actions.Insert(ActionRepairPart);
		//actions.Insert(ActionEmptyBarrel);
		
		// Interact actions
		actions.Insert(ActionPullOutPlug);
		actions.Insert(ActionUnplugThisByCord);
		actions.Insert(ActionTurnOnWhileOnGround);
		actions.Insert(ActionTurnOffWhileOnGround);
		actions.Insert(ActionTurnOnPowerGenerator);
		actions.Insert(ActionTurnOffPowerGenerator);
		actions.Insert(ActionTurnOnSpotlight);
		actions.Insert(ActionTurnOffSpotlight);
		actions.Insert(ActionITest);
		actions.Insert(ActionRemovePlant);
		actions.Insert(ActionHarvestCrops);
		actions.Insert(ActionFoldObject);
		actions.Insert(ActionRepositionPluggedItem);
		//actions.Insert(ActionPickBerry);
		actions.Insert(ActionTurnOnTransmitterOnGround);
		actions.Insert(ActionTurnOffTransmitterOnGround);
		actions.Insert(ActionTuneFrequencyOnGround);
		actions.Insert(ActionToggleTentOpen);
		actions.Insert(ActionPackTent);
		//actions.Insert(ActionRepackTent);
		actions.Insert(ActionOpenBarrel);
		actions.Insert(ActionCloseBarrel);
		actions.Insert(ActionOpenBarrelHoles);
		actions.Insert(ActionCloseBarrelHoles);
		actions.Insert(ActionTakeFireplaceIndoor);
		actions.Insert(ActionTakeOvenIndoor);
		actions.Insert(ActionOpenDoors);
		actions.Insert(ActionCloseDoors);
		actions.Insert(ActionEnterLadder);
		actions.Insert(ActionExitLadder);
		actions.Insert(ActionGetInTransport);
		actions.Insert(ActionTakeItem);
		actions.Insert(ActionDetachPowerSourceFromPanel);
		actions.Insert(ActionSortAmmoPile);
		actions.Insert(ActionSwapItemToHands);
		actions.Insert(ActionTakeItemToHands);
		actions.Insert(ActionLockedDoors);
		actions.Insert(ActionTakeFireplaceFromBarrel);
		actions.Insert(ActionOpenCarDoors);
		actions.Insert(ActionCloseCarDoors);
		actions.Insert(ActionWashHandsWellOne);
		actions.Insert(ActionWashHandsWaterOne);
		actions.Insert(ActionGetOutTransport);
		actions.Insert(ActionAnimateSeats); //<- action doesn't work well, add it later
		actions.Insert(ActionAnimateCarSelection); //<- action doesn't work well, add it later
		actions.Insert(ActionDetach);
		actions.Insert(ActionOpenFence);
		actions.Insert(ActionCloseFence);
		actions.Insert(ActionNextCombinationLockDialOnTarget);
		actions.Insert(ActionTakeMaterialToHands);
		actions.Insert(ActionCheckPulse);
		actions.Insert(ActionUnpin);
		actions.Insert(ActionPin);
		actions.Insert(ActionTakeHybridAttachment);
		actions.Insert(ActionTakeHybridAttachmentToHands);
		actions.Insert(ActionWorldFlagActionSwitch);
		actions.Insert(ActionRaiseFlag);
		actions.Insert(ActionLowerFlag);
		//vehicle actions
		actions.Insert(ActionSwitchLights);
		
		//Instant action - debug
		actions.Insert(ActionDebug);
			
		//Weapon
		actions.Insert(FirearmActionAttachMagazine);
		actions.Insert(FirearmActionLoadMultiBullet);
		actions.Insert(FirearmActionLoadBullet);
		
		actions.Insert(ActionLoadMagazineQuick);
		actions.Insert(FirearmActionAttachMagazineQuick);
		actions.Insert(FirearmActionMechanicManipulate);
		actions.Insert(FirearmActionUnjam);
		
		actions.Insert(FirearmActionDetachMagazine);		
		//actions.Insert(ActionAdvancedDetachMagazine);
		
		actions.Insert(FirearmActionLoadBulletQuick);
		actions.Insert(FirearmActionLoadMultiBulletQuick);
		actions.Insert(FirearmActionLoadMultiBulletRadial);

		actions.Insert(ActionPullBodyFromTransport);
		
	}
};