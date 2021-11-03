// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
//#include "HiddenWordList.h";

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    PrintLine(TEXT("%i"), FMath::RandRange(0, 10));

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    // PrintLine(TEXT("The number of possible words is %i"), Words.Num());
    // PrintLine(TEXT("The first 5 valid words are:"));
    GetValidWords(Words);

    // PrintLine(TEXT("The number of valid words is: %i"), GetValidWords(Words).Num());
    // PrintLine(TEXT("ValidWord - 1 is = %i"), GetValidWords(Words).Num() - 1);
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

    // for (int32 i = 0; i < ValidWords.Num(); i++)
    // {
    //     PrintLine(TEXT("%s."), *ValidWords[i]);
    // }

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
   
    HiddenWord = GetValidWords(Words)[FMath::RandRange(0, GetValidWords(Words).Num() - 1)];
    bGameOver = false;
    Lives = HiddenWord.Len();

    // What's the difference between the dot operator and the :: operator?
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives left."), Lives);
    PrintLine(TEXT("Type in your guess and\npress enter to continue..."));

    //const TCHAR HW[] = TEXT("pages");
    //const TCHAR HW[] = {TEXT('p'), TEXT('a'), TEXT('g'), TEXT('e'), TEXT('s'), TEXT('\0')};
    //HW;
    //PrintLine(TEXT("Character 1 of the hidden word is: %c"), HiddenWord[0]);
    //PrintLine(TEXT("The 4th character of HW is: %c"), HW[3]); // Should print 'e'.
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

    // int32 Index = 0;
    // for (Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
    // {
    //     if (Word[Index] == Word[Comparison])
    //     {
    //         return false;
    //     }
    // }

    // for (int32 Index = 0; Index < Word.Len(); Index++)
    // {
    //     PrintLine(TEXT("%c"), Word[Index]);
    // }
}