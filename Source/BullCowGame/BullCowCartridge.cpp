// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts.
{
    Super::BeginPlay();
    
    PrintLine(TEXT("%i"), FMath::RandRange(0, 10));

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    GetValidWords(Words);

    SetupGame();
}

TArray <FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordsList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordsList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter.
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
   
    HiddenWord = GetValidWords(Words)[FMath::RandRange(0, GetValidWords(Words).Num() - 1)];
    bGameOver = false;
    Lives = HiddenWord.Len();

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives left."), Lives);
    PrintLine(TEXT("Type in your guess and\npress enter to continue..."));

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
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    PrintLine(TEXT("Lost a life!"));
    PrintLine(TEXT("%i"), --Lives);

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    // To do: Show the player Bulls and Cows

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