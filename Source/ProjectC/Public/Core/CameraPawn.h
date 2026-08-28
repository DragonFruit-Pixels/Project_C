// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * El cuerpo del jugador: una cámara, y nada más.
 *
 * **Esto es la posesión del juego.** El `PlayerController` posee este pawn y no posee nunca a un
 * personaje; las figuras se eligen con el mouse. La decisión y su costo están en
 * design/architecture/04-mapa-de-clases.md, "El jugador no tiene cuerpo": el input de un táctico
 * no es "mover mi pawn", es clickear una casilla, y con posesión rotativa había dos rutas de
 * input donde alcanza con una.
 *
 * **Tick, a propósito.** El proyecto declara que un juego por turnos con fases explícitas no
 * necesita Tick (01-por-donde-se-empieza.md). Esa regla es sobre las **reglas**: nada del estado
 * del juego puede depender del paso del tiempo. Una cámara es presentación pura y no toca estado,
 * y sin interpolación el zoom salta y el pan se siente roto. Es una de las dos excepciones del
 * proyecto, y las dos están de este lado de la línea.
 *
 * Cámara táctica, no orbital libre: el pitch es fijo y sólo gira en yaw. Un tablero se lee desde
 * arriba, y dejar inclinar la cámara sólo habilita ángulos desde los que no se entiende el mapa.
 */
UCLASS()
class PROJECTC_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();

	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Desplaza el punto que la cámara mira, en el plano del tablero.
	 *
	 * El eje llega **relativo a la cámara**, no al mundo: después de orbitar, "adelante" sigue
	 * siendo hacia donde mira la cámara. Es lo que hace que W no se sienta arbitrario.
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddPanInput(const FVector2D& Axis);

	/** Positivo acerca. Se aplica al toque porque la rueda es discreta; lo que interpola es el brazo. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddZoomInput(float Axis);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddOrbitInput(float Axis);

	UFUNCTION(BlueprintPure, Category = "Camera")
	UCameraComponent* GetCamera() const { return Camera; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	// --- Perillas. El Blueprint hijo las tunea sin recompilar. ---

	/** uu por segundo con el brazo en su largo por defecto. Se escala con el zoom: ver Tick. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "100"))
	float PanSpeed = 1800.0f;

	/** uu de brazo por muesca de rueda. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "10"))
	float ZoomStep = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "10"))
	float OrbitSpeed = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "100"))
	float MinArmLength = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "200"))
	float MaxArmLength = 4500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "200"))
	float DefaultArmLength = 1800.0f;

	/** Negativo mira hacia abajo. -55 deja ver las caras de las figuras y el layout a la vez. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "-89", ClampMax = "-10"))
	float Pitch = -55.0f;

	/** Cuánto persigue la cámara a su objetivo. Más alto = más rígido. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "1"))
	float InterpSpeed = 10.0f;

private:
	/** Objetivos que Tick persigue. El input escribe acá; nunca mueve el actor directo. */
	FVector TargetLocation = FVector::ZeroVector;
	float TargetArmLength = 0.0f;
	float TargetYaw = 0.0f;

	/**
	 * Eje de pan acumulado desde el último Tick.
	 *
	 * Enhanced Input dispara `Triggered` una vez por frame mientras la tecla está apretada, pero
	 * no entrega DeltaTime. Guardar el eje y consumirlo en Tick es lo que hace que la velocidad
	 * no dependa del framerate.
	 */
	FVector2D PendingPan = FVector2D::ZeroVector;
	float PendingOrbit = 0.0f;
};
