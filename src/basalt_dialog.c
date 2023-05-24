#include "basalt_extra.h"
#define DIALOG_SKIN

typedef struct GameDialog {
    DialogSequence* sequences;
    usize count;
    usize capacity;
} GameDialog;
static GameDialog Dialog = { 0 };

typedef struct SDialogProgress {
    DialogSequence sequence;
    bool isSpeaking;
    usize lineIndex;
    double startTime;
} SDialogProgress;
static SDialogProgress DialogProgress = { 0 };

DIALOG_SKIN bool DrawDefaultDialogBox(const char* dialog, StringArray keywords, Texture canvas, float timeSince)
{
    if (IsKeyPressed(KEY_X))
    {
        return true;
    }
    return false;
}

func bool GetDialogSequenceByName(const char* name, DialogSequence* result);
BASALT void StartDialogSequence(const char* dialog)
{
    DialogSequence sequence;
    if (!GetDialogSequenceByName(dialog, &sequence)) {
        WARN("No registered dialog sequences named %s !", dialog);
        return;
    }

    if (DialogProgress.sequence.name && TextIsEqualNoCase(DialogProgress.sequence.name, dialog))
    {
        WARN("Already speaking, can't start dialog %s", dialog);
        return;
    }

    // start speaking
    memset(&DialogProgress, 0, sizeof(DialogProgress));
    memcpy(&DialogProgress.sequence, &sequence, sizeof(DialogSequence));
    DialogProgress.isSpeaking = true;
    DialogProgress.startTime = GetTimeElapsed();
    INFO("> Started dialog sequence %s", dialog);
}

BASALT inline bool UpdateAndRenderDialogBoxes(Texture canvas)
{
    return UpdateAndRenderCustomDialogBoxes(canvas, DrawDefaultDialogBox);
}

BASALT bool UpdateAndRenderCustomDialogBoxes(Texture canvas, DialogBoxDrawerFunc drawingFunc)
{
    if (DialogProgress.isSpeaking)
    {
        DialogLine line = DialogProgress.sequence.lines[DialogProgress.lineIndex];
        float elapsed = (float)(GetTimeElapsed() - DialogProgress.startTime);

        // advance to next line if confirmed
        bool confirmed = drawingFunc(line.text, line.keywords, canvas, elapsed);
        if (confirmed)
        {
            DialogProgress.lineIndex++;
            if (DialogProgress.lineIndex >= DialogProgress.sequence.lineCount)
            {
                DialogProgress.isSpeaking = false;
                INFO("< Ended dialog sequence %s", DialogProgress.sequence.name);
            }
            else
            {
                INFO(">>> %s", DialogProgress.sequence.lines[DialogProgress.lineIndex].text);
            }
        }
    }
    return DialogProgress.isSpeaking;
}

BASALT StringArray ExtractDialogKeywords(const char* line)
{
    StringArray result = InitStringArray();

    usize keywordStart = 0;
    for (usize i = 0; i < strlen(line); i++) {
        char token = line[i];
        if (token == '{') {
            if (keywordStart > 0) {
                ERR("Duplicate start brace for dialog line %s", line);
            }
            keywordStart = i + 1;
        } else if (token == '}' || token == '|') {
            if (keywordStart == 0) {
                ERR("No start brace found for dialog line %s", line);
            }

            // copy the keyword over and place it into the stringarray
            usize len = i - keywordStart;
            char* slice = malloc(len + 1);
            memcpy(slice, &line[keywordStart], len);
            slice[len] = '\0';
            StoreString(&result, slice);
            free(slice);

            if (token == '}')  // reached the end
            {
                break;
            } else {
                keywordStart = i + 1;
            }
        }
    }
    return result;
}

BASALT StringArray ExtractDialogLines(const char* lines)
{
    char* buffer = CloneText(lines);

    StringArray result = InitStringArray();
    const char* SEP = "\\";
    char* line = strtok(buffer, SEP);
    while (line != NULL) {
        StripText(line);
        StoreString(&result, line);
        line = strtok(NULL, SEP);
    }

    free(buffer);
    return result;
}

func bool GetDialogSequenceByName(const char* name, DialogSequence* result)
{
    if (Dialog.sequences) {
        for (usize i = 0; i < Dialog.count; i++) {
            DialogSequence dialog = Dialog.sequences[i];
            if (TextIsEqualNoCase(dialog.name, name)) {
                memcpy(result, &dialog, sizeof(DialogSequence));
                return true;
            }
        }
    }
    return false;
}

BASALT void RegisterDialogSequence(const char* name, const char* lines)
{
    if (!Dialog.sequences) {
        Dialog.capacity = 20;
        Dialog.sequences = malloc(Dialog.capacity * sizeof(DialogSequence));
    }
    if (Dialog.count == Dialog.capacity) {
        Dialog.capacity *= 2;
        Dialog.sequences = realloc(Dialog.sequences, Dialog.capacity);
    }

    DialogSequence seq = { 0 };
    seq.name = name;

    // extract lines from compact 'lines' parameter
    StringArray sa = ExtractDialogLines(lines);
    for (usize i = 0; i < sa.count; i++) {
        if (i >= MAX_LINES_PER_SEQUENCE) {
            WARN("Too many lines for a single dialog sequence, increase the fixed array size!");
            break;
        }
        DialogLine* line = &seq.lines[i];
        line->text = strdup(sa.strings[i]);
        StringArray keywords = ExtractDialogKeywords(line->text);
        memcpy(&line->keywords, &keywords, sizeof(StringArray));
    }
    seq.lineCount = sa.count;
    DisposeStringArray(&sa);

    memcpy(&Dialog.sequences[Dialog.count++], &seq, sizeof(DialogSequence));
    DEBUG("Registered dialog line %s with %d lines.", name, seq.lineCount);
}

// ==== Disposing ===
func void DisposeDialogLine(DialogLine line)
{
    if (line.text) {
        free(line.text);
    }
    DisposeStringArray(&line.keywords);
}

func void DisposeDialogSequence(DialogSequence sequence)
{
    for (usize i = 0; i < sequence.lineCount; i++) {
        DisposeDialogLine(sequence.lines[i]);
    }
}

BASALT void ClearDialogSequences()
{
    if (Dialog.sequences) {
        for (usize i = 0; i < Dialog.count; i++) {
            DisposeDialogSequence(Dialog.sequences[i]);
        }
        free(Dialog.sequences);
    }
    memset(&Dialog, 0, sizeof(GameDialog));
    INFO("Cleared dialog sequences!");
}
