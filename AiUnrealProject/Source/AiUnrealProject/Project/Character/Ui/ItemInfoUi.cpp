// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfoUi.h"

#include "ContentBrowserItemData.h"
#include "HttpModule.h"
#include "IImageWrapperModule.h"
#include "JsonObjectConverter.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Interfaces/IHttpResponse.h"
#include "Project/ApiObject/Npc/ApiItemObject.h"

class IImageWrapper;
class FHttpModule;
class USizeBox;
class UVerticalBox;
class UHorizontalBox;
class UImage;

void UItemInfoUi::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemInfoUi::LoadItems()
{
    UE_LOG(LogTemp, Display, TEXT("Loading items"));

    UApiItemObject* Api = NewObject<UApiItemObject>();
    Api->OnItemInfoResponse.BindLambda([this](FString String)
    {
        FItemRows ItemRows;
        if (!FJsonObjectConverter::JsonObjectStringToUStruct(String, &ItemRows))
        { UE_LOG(LogTemp, Warning, TEXT("❌ Failed to parse ItemRows")); return; }

        VScroll->ClearChildren();
        ButtonItemMap.Empty();

        for (const FItemRow& Item : ItemRows.response)
        {
            // 버튼 + 한 줄(Row)
            UButton* ClickButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
            UHorizontalBox* RowBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());

            // 🔸 버튼 투명 처리
            FButtonStyle TransparentStyle;
            TransparentStyle.SetNormal(FSlateNoResource());
            TransparentStyle.SetHovered(FSlateNoResource());
            TransparentStyle.SetPressed(FSlateNoResource());
            TransparentStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
            TransparentStyle.Hovered.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.1f));
            TransparentStyle.Pressed.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0.2f));
            ClickButton->SetStyle(TransparentStyle);

              // 🔹 UImage 위젯 먼저 생성 (placeholder)
            UImage* Icon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
            UTexture2D* DummyTex = UTexture2D::CreateTransient(64, 64);
            Icon->SetBrushFromTexture(DummyTex);
            Icon->SetColorAndOpacity(FLinearColor::Gray);

            USizeBox* IconBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
            IconBox->SetWidthOverride(80.f);
            IconBox->SetHeightOverride(80.f);
            IconBox->AddChild(Icon);
            RowBox->AddChildToHorizontalBox(IconBox);

            // 🔸 2차 요청: image_url → 이미지 다운로드
            FHttpModule* Http = &FHttpModule::Get();
            TSharedRef<IHttpRequest, ESPMode::ThreadSafe> ImgRequest = Http->CreateRequest();
            FString Str = Item.image_url.Replace(TEXT("\\"), TEXT("/"));
            ImgRequest->SetURL(FString::Printf(TEXT("http://127.0.0.1:8000/%s"), *Str));
            ImgRequest->SetVerb("GET");

            // 내부 BindLambda (중첩 콜백)
            ImgRequest->OnProcessRequestComplete().BindLambda([Icon](FHttpRequestPtr ImgReq, FHttpResponsePtr ImgRes, bool bOk)
            {
                if (!bOk || !ImgRes.IsValid())
                {
                    UE_LOG(LogTemp, Error, TEXT("이미지 다운로드 실패"));
                    return;
                }

                const TArray<uint8>& Data = ImgRes->GetContent();

                IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
                TSharedPtr<IImageWrapper> Wrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

                if (Wrapper.IsValid() && Wrapper->SetCompressed(Data.GetData(), Data.Num()))
                {
                    TArray64<uint8> RawData;
                    if (Wrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
                    {
                        UTexture2D* Texture = UTexture2D::CreateTransient(
                            Wrapper->GetWidth(),
                            Wrapper->GetHeight(),
                            PF_B8G8R8A8
                        );
                 
                        void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
                        FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
                        Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
                        Texture->UpdateResource();
                 
                        AsyncTask(ENamedThreads::GameThread, [Icon, Texture]()
                        {
                            Icon->SetBrushFromTexture(Texture);
                            Icon->SetColorAndOpacity(FLinearColor::White);
                            UE_LOG(LogTemp, Warning, TEXT("✅ 아이템 이미지 로드 완료"));
                        });
                    }
                }
            });

            ImgRequest->ProcessRequest();

            // 🔸 텍스트 셀 생성 함수
            auto MakeTextCell = [&](const FString& Value, float Width, FLinearColor Color = FLinearColor::White)
            {
                USizeBox* CellBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
                CellBox->SetWidthOverride(Width);

                UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
                Text->SetText(FText::FromString(Value));
                Text->SetColorAndOpacity(FSlateColor(Color));
                Text->SetJustification(ETextJustify::Left);
                Text->SetAutoWrapText(false);
                Text->SetMargin(FMargin(5.f, 0.f, 5.f, 0.f));
                // 폰트 크기 지정 (한글 깨짐 방지를 위해 지정하지 않아도 됨)
                // Text->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));
                CellBox->AddChild(Text);

                return CellBox;
            };

            // 🔸 등급 색상
            FLinearColor RarityColor = FLinearColor::White;
            if (Item.Rarity.Contains(TEXT("전설")))      RarityColor = FLinearColor(1.f, 0.85f, 0.2f);
            else if (Item.Rarity.Contains(TEXT("희귀"))) RarityColor = FLinearColor(0.3f, 0.6f, 1.f);
            else if (Item.Rarity.Contains(TEXT("일반"))) RarityColor = FLinearColor(0.85f, 0.85f, 0.85f);

            // 🔸 엑셀 스타일 컬럼 구성
            RowBox->AddChildToHorizontalBox(MakeTextCell(Item.Name, 300.f));
            RowBox->AddChildToHorizontalBox(MakeTextCell(Item.Rarity, 120.f, RarityColor));
            RowBox->AddChildToHorizontalBox(MakeTextCell(Item.Type, 100.f, FLinearColor(0.7f, 1.f, 0.7f)));

            // 버튼에 RowBox 추가
            ClickButton->AddChild(RowBox);
            VScroll->AddChild(ClickButton);

            // 버튼별 아이템 데이터 저장
            ButtonItemMap.Add(ClickButton, Item);

            // 클릭 이벤트 바인딩
            ClickButton->OnClicked.AddDynamic(this, &UItemInfoUi::OnItemClick);
        }
    });

    Api->ItemInfoResponse();
}

void UItemInfoUi::OnItemClick()
{
    // 클릭된 버튼 탐색
    for (auto& Pair : ButtonItemMap)
    {
        UButton* Button = Pair.Key;
        if (Button->HasKeyboardFocus()) // 현재 포커스 받은 버튼
        {
            const FItemRow& Item = Pair.Value;

            if (DetailPanel)
            {
                DetailPanel->ClearChildren();

                // 제목
                UTextBlock* Title = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
                Title->SetText(FText::FromString(Item.Name));
                Title->SetColorAndOpacity(FSlateColor(FLinearColor::White));

                // 본문 설명
                UTextBlock* Desc = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
                Desc->SetText(FText::FromString(Item.Description));
                Desc->SetAutoWrapText(true);
                Desc->SetMargin(FMargin(10.f));

                DetailPanel->AddChild(Title);
                DetailPanel->AddChild(Desc);
            }
            break;
        }
    }
}

