using UnityEngine;
using UnityEngine.InputSystem;

public class ShootScript : MonoBehaviour
{
    public float launchForce = 15f;
    public GameObject Arrow;
    public Transform arrowSpawnPoint;

    void Update()
    {
        if (Keyboard.current != null &&
            Keyboard.current.spaceKey.wasPressedThisFrame)
        {
            Shoot();
        }
    }

    void Shoot()
    {
        GameObject arrow = Instantiate(
            Arrow,
            arrowSpawnPoint.position,
            arrowSpawnPoint.rotation
        );

        Rigidbody2D rb = arrow.GetComponent<Rigidbody2D>();

        if (rb == null)
        {
            Debug.LogError("Arrow prefab has no Rigidbody2D!");
            return;
        }

        rb.bodyType = RigidbodyType2D.Dynamic;
        rb.gravityScale = 1f;
        rb.linearVelocity = arrowSpawnPoint.right * launchForce;

        Destroy(arrow, 10f);
    }
}