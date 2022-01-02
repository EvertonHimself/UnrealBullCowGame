// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
//#include "HiddenWordList.h";

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    
    //FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word) { return Word.Len() < 5; });
    /* FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
    {
        return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word);
    }); */
    
    Isograms = GetValidWords(Words);
    SetupGame();
}

TArray <FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordsList) const
{
    TArray<FString> ValidWords;

    // Former "for" parameters: int32 i = 0; i < WordsList.Num(); i++.
    for (FString Word : WordsList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        
            // I don't know WHY it only works with the "*"" operator (*Words).
            // The * is called "dereference", and i have no idea why and what it is for.
            //PrintLine(TEXT("%s"), *Words[i]);
        }
    }

    return ValidWords;
}

// What's the difference between & and *? (Kinda answered on 77. Introduction To References)
void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver == true)
    {
        ClearScreen();
        SetupGame();
        return;
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to Bull-Cow Game!"));
   
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    bGameOver = false;
    Lives = HiddenWord.Len();

    // What's the difference between the dot operator and the :: operator?
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives left."), Lives);
    PrintLine(TEXT("Type in your guess and\npress enter to continue..."));

    // I've forgot why we have to use the *HiddenWord...
    PrintLine(TEXT("The HiddenWord is: %s.\nIt is %i characters long"), *HiddenWord, HiddenWord.Len()); // Debug line.
    IsIsogram(HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again, \nyou have %i lives remaining"), Lives);
        return;
    }

    if (!IsIsogram(Guess))
    {
        /* code */
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    PrintLine(TEXT("Lost a life!"));

    // Using Lives-- will show the value of Lives BEFORE it was decremented. Ex: It will show
    // 5 even though the actual value is 4.
    PrintLine(TEXT("%i"), --Lives);

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    int32 Bulls, Cows;
    GetBullCows(Guess, Bulls, Cows);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    
    return true;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    // for every index Guess is same as index Hidden, BullCount ++.
    // if not a bull was it a cow? if yes CowCount ++.
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                CowCount++;
            }
        }
    }
}