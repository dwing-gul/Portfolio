using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;

public class HellBeastRecognition : MonoBehaviour
{
    [SerializeField]
    private HellBeast Owner;
    private bool isEnter;

    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            isEnter = true;
            if (!Owner.mbDead)
            {
                Vector2 direction = collision.transform.position - Owner.transform.position;
                if (direction.x < 0)
                {
                    Owner.transform.rotation = Quaternion.Euler(0, 0, 0);
                }
                else
                {
                    Owner.transform.rotation = Quaternion.Euler(0, 180, 0);
                }
            }
            Owner.LongDistanceAttack(isEnter);
            UnityEngine.Debug.Log("PlayerEnter");
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            isEnter = false;
            Owner.LongDistanceAttack(isEnter);
        }
    }
}
