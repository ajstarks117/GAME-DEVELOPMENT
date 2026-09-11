using UnityEngine;

public class ArrowScript : MonoBehaviour
{
    Rigidbody2D rb;
    bool hasHit = false;

    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }

    void Update()
    {
        if (!hasHit)
        {
            TrackMovement();
        }
    }

    void TrackMovement()
    {
        Vector2 velocity = rb.linearVelocity;

        float angle = Mathf.Atan2(
            velocity.y,
            velocity.x
        ) * Mathf.Rad2Deg;

        transform.rotation = Quaternion.AngleAxis(
            angle,
            Vector3.forward
        );
    }

    void OnCollisionEnter2D(Collision2D collision)
    {
        if(hasHit) return;

        EnemyScript enemy = collision.gameObject.GetComponent<EnemyScript>();

        if(enemy != null)
        {
            enemy.TakeDamage(1);
            Destroy(gameObject);
            return;
        }

        hasHit = true;

        rb.linearVelocity = Vector2.zero;
        rb.bodyType = RigidbodyType2D.Kinematic;
    }
}