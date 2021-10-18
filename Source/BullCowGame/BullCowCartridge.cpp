// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    SetupGame();

    // I've forgot why we have to use the *HiddenWord...
    PrintLine(TEXT("The HiddenWord is: %s.\nIt is %i characters long"), *HiddenWord, HiddenWord.Len()); // Debug line.
}

// What's the difference between & and *?
void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver == true)
    {
        ClearScreen();
        SetupGame();
        return;
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to Bull-Cow Game!"));
   
    HiddenWord = TEXT("pages");
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

    IsIsogram(HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(FString Guess)
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

bool UBullCowCartridge::IsIsogram(FString Word) const
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